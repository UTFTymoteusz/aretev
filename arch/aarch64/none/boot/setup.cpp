#include "art/arch/mm.hpp"
#include "art/data.hpp"
#include "art/int.hpp"
#include "art/mm.hpp"

#include "mm/boot.hpp"
#include "registers.hpp"

#define section_args(name) (name##_vstart, name##_start, name##_vend - name##_vstart)

namespace art {
    section(".boot.data") aligned(arch::mm::PG_SIZE) u64 l1_lower[512];
    section(".boot.data") aligned(arch::mm::PG_SIZE) u64 l1_upper[512];
    section(".boot.data") aligned(arch::mm::PG_SIZE) u64 l2_array[4][512];
    section(".boot.data") aligned(arch::mm::PG_SIZE) u64 l3_array[12][512];
    section(".boot.data") int l2off = 0;
    section(".boot.data") int l3off = 0;
    section(".boot.data") usz fault_vector[512];

    static_assert(sizeof(l1_lower) == 4096);
    static_assert(sizeof(l2_array[0]) == 4096);
    static_assert(sizeof(l3_array[0]) == 4096);

    extern "C" void aarch64_faultjmp();

    section(".boot.text") int index(addr_t vaddr, int level) {
        using namespace art::arch::mm;
        return vaddr >> (PG_OFFSET + (level - 1) * PG_SHIFT) & ((1 << PG_SHIFT) - 1);
    }

    extern "C" section(".boot.text") void aarch64_fault() {
        void (*instruction)();
        usz instraddr;
        void (*addr_ptr)();
        usz addr;

        asm volatile("mrs %0, elr_el1" : "=r"(instruction));
        asm volatile("mrs %0, elr_el1" : "=r"(instraddr));
        asm volatile("mrs %0, far_el1" : "=r"(addr_ptr));
        asm volatile("mrs %0, far_el1" : "=r"(addr));

        while (true)
            ;
    }

    /**
     * @brief Maps a section.
     */
    section(".boot.text") void aarch64_section(addr_t vaddr, phys_t paddr, usz size) {
        int  l1i = index(vaddr, 4);
        u64* l1  = l1i == 0 ? l1_lower : l1_upper;

        u64* l2 = (u64*) (l1[index(vaddr, 3)] & ~(arch::mm::PG_SIZE - 1));
        if (!l2) {
            l1[index(vaddr, 3)] = (u64) &l2_array[l2off] | 0b10000000011;
            l2                  = (u64*) l2_array[l2off];
            l2off++;
        }

        u64* l3 = (u64*) (l2[index(vaddr, 2)] & ~(arch::mm::PG_SIZE - 1));
        if (!l3) {
            l2[index(vaddr, 2)] = (u64) &l3_array[l3off] | 0b10000000011;
            l3                  = (u64*) l3_array[l3off];
            l3off++;
        }

        for (usz i = 0; i < alfit(size, arch::mm::PG_SIZE); i++) {
            l3[index(vaddr, 1)] = paddr | 0b10000000011;
            vaddr += arch::mm::PG_SIZE;
            paddr += arch::mm::PG_SIZE;
        }
    }

    /**
     * @brief Sets up a basic page table.
     */
    section(".boot.text") void aarch64_pgtable(phys_t dtb) {
        using namespace mm;

        // keep in mind that ARM paging levels are in reverse compared to art

        l1_lower[arch::mm::PG_LO] = (u64) &l1_lower | 0b10000000011;
        l1_upper[arch::mm::PG_LO] = (u64) &l1_upper | 0b10000000011;

        aarch64_section section_args(b_boot_text);
        aarch64_section section_args(b_boot_rodata);
        aarch64_section section_args(b_boot_data);
        aarch64_section section_args(b_boot_bss);
        aarch64_section section_args(b_kernel_text);
        aarch64_section section_args(b_kernel_rodata);
        aarch64_section section_args(b_kernel_data);
        aarch64_section section_args(b_kernel_bss);
        aarch64_section section_args(b_kernel_init);

        aarch64_section(dtb, dtb, 0x100000);
    }

    extern "C" section(".boot.text") void aarch64_setup(phys_t dtb) {
        asm volatile("msr vbar_el1, %0" : : "r"(aarch64_faultjmp));

        aarch64_pgtable(dtb);

        asm volatile("msr mair_el1, %0" : : "r"(0x00000000));

        u64 id_aa64mmfr0_el1;
        u64 cpacr_el1;

        asm volatile("mrs %0, id_aa64mmfr0_el1" : "=r"(id_aa64mmfr0_el1));

        asm volatile("mrs %0, cpacr_el1" : "=r"(cpacr_el1));
        cpacr_el1 |= (0b11 << 20);
        asm volatile("msr cpacr_el1, %0" : : "r"(cpacr_el1));

        tcr_el1 tcr_el1 = {};

        tcr_el1.t0sz(25)
            .epd0(true)
            .irgn0(TCR_CHWBRAWA)
            .orgn0(TCR_CHWBRAWA)
            .sh0(TCR_SHINNER)
            .tg0(TCR_GRAN4K)
            .tbi0(false)
            .t1sz(25)
            .epd1(true)
            .irgn1(TCR_CHWBRAWA)
            .orgn1(TCR_CHWBRAWA)
            .sh1(TCR_SHINNER)
            .tg1(TCR_GRAN4K)
            .tbi1(false)
            .ips(id_aa64mmfr0_el1 & 0b00001111);

        asm volatile("msr tcr_el1, %0" : : "r"(tcr_el1.value));
        asm volatile("msr ttbr0_el1, %0" : : "r"((u64) &l1_lower | 0x01));
        asm volatile("msr ttbr1_el1, %0" : : "r"((u64) &l1_upper | 0x01));
        asm volatile("dsb ish;");
        asm volatile("isb;");

        sctlr_el1 sctlr_el1 = {};

        asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr_el1.value));

        sctlr_el1.value |= 0xC00800;
        sctlr_el1.m(true)
            .a(false)
            .c(false)
            .sa(false)
            .sa0(false)
            .i(false)
            .wxn(false)
            .ee(false)
            .e0e(false);

        asm volatile("msr sctlr_el1, %0" : : "r"(sctlr_el1.value));
        asm volatile("isb;");
    }
}