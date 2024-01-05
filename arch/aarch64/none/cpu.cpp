#include "art/cpu.hpp"

#include "art/int.hpp"
#include "art/mm.hpp"

namespace art {
    void cpu::mm_flush() {
        asm volatile("tlbi VMALLE1");
    }

    void cpu::mm_flush(void* addr) {
        asm volatile("tlbi VAAE1, %0" : : "r"(addr));
    }

    void cpu::mm_flush(void* addr, usz count) {
        for (usz off = 0; off < count; off++)
            cpu::mm_flush((void*) ((addr_t) addr + off * arch::mm::PG_SIZE));
    }

    void cpu::halt() {
        asm volatile("mrs x9, daif;");
        asm volatile("and x9, x9, ~0x03C0");
        asm volatile("msr daif, x9;");
        asm volatile("wfi;");
    }

    void cpu::dg_fault() {
        asm volatile(".inst 0xe7f000f0");
    }

    void cpu::no_interrupts() {
        asm volatile("mrs x9, daif;");
        asm volatile("and x9, x9, ~0x00C0");
        asm volatile("msr daif, x9;");
    }

    void cpu::interrupts() {
        asm volatile("mrs x9, daif;");
        asm volatile("orr x9, x9, 0x00C0");
        asm volatile("msr daif, x9;");
    }

    bool cpu::check_interrupts() {
        usz flags;

        asm volatile("mrs %0, daif;" : "=r"(flags));

        return flags & 0x00C0;
    }

    void cpu::use(mm::pagetbl* pagetbl, bool kernel) {
        if (kernel)
            asm volatile("msr ttbr1_el1, %0" : : "r"(pagetbl->root() | 0x01));
        else
            asm volatile("msr ttbr0_el1, %0" : : "r"(pagetbl->root() | 0x01));
    }
}