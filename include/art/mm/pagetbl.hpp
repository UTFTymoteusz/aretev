#pragma once

#include "art/int.hpp"

namespace art::mm {
    /**
     * @brief Paging allocation flags.
     */
    enum pgflags {
        PG_NONE    = 0b00000000,
        PG_PRESENT = 0b00000001,
        PG_READ    = 0b00000010,
        PG_WRITE   = 0b00000100,
        PG_EXEC    = 0b00001000,
        PG_GLOBAL  = 0b00010000,
        PG_NOCACHE = 0b00100000,
        PG_RW      = PG_READ | PG_WRITE,
        PG_RWX     = PG_READ | PG_WRITE | PG_EXEC,
        PG_RX      = PG_READ | PG_EXEC,
    };

    class pagetbl;

    /**
     * @brief A class that manages a page table.
     */
    class pagetbl {
        public:
        addr_t start;
        addr_t end;

        pagetbl();

        /**
         * @brief Constructs a new pagetbl object.
         * @param start Address space start.
         * @param end   Address space end.
         */
        pagetbl(addr_t start, addr_t end, pagetbl* parent);

        /**
         * @brief Maps a physical address to a virtual address.
         * The physical address does not have to be aligned to a page boundary. In that
         * case, this function will return a pointer offset by the same amount. This
         * method accepts a length in bytes, but it will still round up to page
         * granularity.
         * @param paddr Physical address to map to.
         * @param len   Length of the mapping in bytes.
         * @param vaddr Optional virtual address.
         * @param flags Page access flags.
         * @return Allocated virtual address, or nullptr on failure.
         */
        void* map(phys_t paddr, usz len, void* vaddr = nullptr,
                  int flags = PG_READ | PG_PRESENT);

        /**
         * @brief Unmaps the specified virtual address.
         * @param vaddr Virtual address to unmap.
         * @param len   Length of the mapping in bytes.
         */
        void unmap(void* vaddr, usz len);

        constexpr phys_t root() {
            return _root;
        }

        /**
         * @brief Fills up the root pagetbl.
         * This should only ever be used when booting.
         */
        void boot_fill();

        private:
        phys_t   _root;
        pagetbl* _parent = nullptr;

        /**
         * @brief Finds free space for a contiguous mapping.
         * @param count Page count.
         * @return Found virtual address.
         */
        addr_t find(usz count);

        /**
         * @brief Writes a mapping.
         * @param paddr Physical address to map to.
         * @param count Page count.
         * @param vaddr Virtual address.
         * @param flags Page access flags.
         * @param autoincrement Whether to increment paddr by PG_SIZE after each
         * page entry modified.
         * @param autoincrement Whether to modify the table referenced by LL_LOB.
         */
        void write(phys_t paddr, usz count, addr_t vaddr, int flags, bool autoincrement,
                   bool boot = false);

        /**
         * @brief Creates the root of a page table and initializes it.
         * @param strap pagetbl to use for initializing the root.
         * @return Physical address of the page table root.
         */
        phys_t create_root(pagetbl* strap);
    };
}