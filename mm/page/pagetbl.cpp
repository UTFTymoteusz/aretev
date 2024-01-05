#include "art/mm/pagetbl.hpp"

#include "art/arch/mm.hpp"
#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/int.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

#include "mm/boot.hpp"

#define section_args(name) \
    name##_start, (name##_vend - name##_vstart + (PG_SIZE - 1)) / PG_SIZE, name##_vstart

namespace art::mm {
    using namespace art::arch::mm;

    /**
     * @brief A struct representing the native flags of a page table entry.
     */
    struct pgeflags {
        /**
         * @brief A convenience macro for creating bit accessors.
         */
#define bit_access(name, flag)                                             \
    constexpr bool name() {                                                \
        return (this->flags & flag) == flag;                               \
    }                                                                      \
    constexpr pgeflags& name(bool name) {                                  \
        this->flags = name ? (this->flags | flag) : (this->flags & ~flag); \
        return *this;                                                      \
    }

        usz flags = 0x00;

        pgeflags() {}

        /**
         * @brief Constructs a pgeflags from pgflags.
         * This converts agnostic flags to native ones.
         * @param art_flags pgflags permission flags.
         */
        pgeflags(int art_flags) {
            present(art_flags & PG_PRESENT);
            ro(!(art_flags & PG_WRITE));
            rw(art_flags & PG_WRITE);
            // nocache(art_flags & PG_NOCACHE);
            // global(art_flags & PG_GLOBAL);
        }

        bit_access(present, PN_PRESENT);
        bit_access(ro, PN_RO);
        bit_access(rw, PN_RW);
        bit_access(user, PN_USER);
#undef bit_access
    };

    /**
     * @brief A combo struct representing a page table entry with it's flags.
     */
    template <typename C>
    struct pgentry {
        C        ent;
        pgeflags flags;
    };

    /**
     * @brief A generic page level class.
     */
    class pglevel {
        public:
        /**
         * @brief Returns a pointer to an entry.
         * @param index Table index.
         * @return Recursive pointer to the table.
         */
        pgentry<pglevel*> get(int index) {
            return {recptr(index), entries[index]};
        }

        /**
         * @brief Returns the raw (physical) address of an entry.
         * @param index Table index.
         * @return Value of the table entry.
         */
        pgentry<phys_t> getp(int index) {
            return {(phys_t) entries[index] & PG_MASK, entries[index]};
        }

        /**
         * @brief Modifies an entry and preserves it's flags.
         * @param index Table index.
         * @param entry Entry value.
         */
        void setphys(int index, phys_t entry) {
            entries[index] = (entries[index] & ~PG_MASK) | entry;
            cpu::mm_flush(recptr(index));
        }

        /**
         * @brief Modifies an entry.
         * @param index Table index.
         * @param entry Entry value.
         * @param flags Native flags to apply to the entry.
         */
        void setphys(int index, phys_t entry, pgeflags flags) {
            entries[index] = entry | flags.flags;
            cpu::mm_flush(recptr(index));
        }

        /**
         * @brief Allocates a table and sets it as an entry.
         * @param index Entry index.
         * @return Recursive pointer to the table.
         */
        pgentry<pglevel*> alloc(int index) {
            setphys(index, ppalloc(PG_SIZE), pgeflags().present(true).rw(true));

            auto ret = get(index);

            cpu::mm_flush(ret.ent);
            memset(ret.ent, '\0', PG_SIZE);

            return ret;
        }

        private:
        phys_t entries[1 << PG_SHIFT];

        pglevel* recptr(int index) {
            addr_t self = (addr_t) this;

            self <<= PG_SHIFT;
            self &= ~((1ul << (PG_OFFSET + PG_SHIFT)) - 1);
            self |= ~((1ul << (PG_OFFSET + PG_SHIFT * PG_LEVELS)) - 1);

            return (pglevel*) (self + (index << 12));
        }
    };

    /**
     * @brief Returns a recursive pointer to the pml4.
     * @return Recursive pointer to the pml4.
     */
    static pglevel* pgaddr() {
        return (pglevel*) PG_LOADDR;
    }

    /**
     * @brief Returns a recursive pointer to the boot-swap pml4.
     * @return Recurisve pointer to the boot-swap pml4.
     */
    static pglevel* pgaddr_boot() {
        return (pglevel*) PG_LOBADDR;
    }

    constexpr int index(addr_t vaddr, int level) {
        return vaddr >> (PG_OFFSET + (level - 1) * PG_SHIFT) & ((1 << PG_SHIFT) - 1);
    }

    pagetbl::pagetbl() {}

    pagetbl::pagetbl(addr_t start, addr_t end, pagetbl* parent) {
        this->start   = start;
        this->end     = end;
        this->_parent = parent;
        this->_root   = this->create_root(_parent);
    }

    void* pagetbl::map(phys_t paddr, usz len, void* vaddr, int flags) {
        usz    offset = aloffset(paddr, PG_SIZE);
        usz    count  = alfit(offset + len, PG_SIZE);
        addr_t addr   = (addr_t) vaddr & PG_MASK;

        flags |= PG_PRESENT;

        if (addr == 0)
            addr = this->find(count);

        this->write(paddr & PG_MASK, count, addr, flags, true);

        cpu::mm_flush(vaddr, count);

        return (void*) (addr + offset);
    }

    void pagetbl::unmap(void* vaddr, usz len) {
        usz    offset = aloffset((addr_t) vaddr, PG_SIZE);
        usz    count  = alfit(offset + len, PG_SIZE);
        addr_t addr   = (addr_t) vaddr & PG_MASK;

        this->write(0x0000, count, addr, PG_NONE, true);
    }

    section(".boot.text") void pagetbl::boot_fill() {
        // usz boot_pages   = alfit(boot_end - alfloor(boot_start, PG_SIZE), PG_SIZE);
        // usz kernel_pages = alfit(kernel_end - alfloor(kernel_start, PG_SIZE), PG_SIZE);

        this->start   = kernelhalf_start;
        this->end     = kernelhalf_end;
        this->_parent = nullptr;
        this->_root   = this->create_root(this);

        pgaddr()->setphys(PG_LOB, _root, pgeflags().present(true).rw(true));

        cpu::mm_flush();

        this->write(section_args(b_boot_text), PG_PRESENT | PG_RX, true, true);
        this->write(section_args(b_boot_rodata), PG_PRESENT | PG_READ, true, true);
        this->write(section_args(b_boot_data), PG_PRESENT | PG_RW, true, true);
        this->write(section_args(b_boot_bss), PG_PRESENT | PG_RW, true, true);
        this->write(section_args(b_kernel_text), PG_PRESENT | PG_RX | PG_GLOBAL, true,
                    true);
        this->write(section_args(b_kernel_rodata), PG_PRESENT | PG_RX | PG_GLOBAL, true,
                    true);
        this->write(section_args(b_kernel_data), PG_PRESENT | PG_RW | PG_GLOBAL, true,
                    true);
        this->write(section_args(b_kernel_bss), PG_PRESENT | PG_RW | PG_GLOBAL, true,
                    true);
        this->write(section_args(b_kernel_init), PG_PRESENT | PG_RW | PG_GLOBAL, true,
                    true);

        cpu::use(this, true);

        pgaddr()->setphys(PG_LOB, 0x0000, pgeflags().present(false));

        cpu::mm_flush();
    }

    addr_t pagetbl::find(usz count) {
        addr_t   target = start + count * PG_SIZE;
        pglevel* top    = pgaddr();
        pglevel* current;

        for (addr_t vaddr = start; vaddr < end;) {
            current = top;

            for (int i = PG_LEVELS; i > 1; i--) {
                auto pgentry = current->get(index(vaddr, i));

                if (!pgentry.flags.present()) {
                    vaddr += PG_SIZE * 1 << (PG_SHIFT * (i - 1));
                    if (vaddr >= target && target >= start)
                        return target - count * PG_SIZE;

                    continue;
                }

                current = pgentry.ent;
            }

            for (int i = 0; i < 1 << PG_SHIFT; i++) {
                if (current->getp(i).ent)
                    target = vaddr + (count + 1) * PG_SIZE;
                else if (vaddr == target)
                    return target - count * PG_SIZE;

                vaddr += PG_SIZE;
            }
        }

        return 0;
    }

    void pagetbl::write(phys_t paddr, usz count, addr_t vaddr, int aflags, bool ainc,
                        bool boot) {
        pgeflags nflags = aflags;
        pglevel* current;

        for (usz i = 0; i < count; i++) {
            if ((vaddr % (1 << PG_SHIFT * 2)) == 0 || i == 0) {
                current = boot ? pgaddr_boot() : pgaddr();

                for (int i = PG_LEVELS; i > 1; i--) {
                    int  eid  = index(vaddr, i);
                    auto next = current->get(eid);

                    if (!next.flags.present())
                        next = current->alloc(eid);

                    current = next.ent;
                }
            }

            current->setphys((vaddr / PG_SIZE) & ((1 << PG_SHIFT) - 1), paddr, nflags);

            if (ainc)
                paddr += PG_SIZE;

            vaddr += PG_SIZE;
        }
    }

    phys_t pagetbl::create_root(pagetbl* strap) {
        phys_t   root  = ppalloc(PG_SIZE);
        pglevel* rootv = (pglevel*) strap->map(root, PG_SIZE, nullptr, PG_RW);

        memset(rootv, '\0', PG_SIZE);

        rootv->setphys(PG_LO, root, pgeflags().present(true).rw(true));
        rootv->setphys(PG_LOB, root, pgeflags().present(true).rw(true));
        strap->unmap(rootv, PG_SIZE);

        return root;
    }
}