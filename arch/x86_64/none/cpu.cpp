#include "art/cpu.hpp"

#include "art/int.hpp"
#include "art/mm.hpp"

namespace art {
    void cpu::mm_flush() {
        asm volatile("mov rax, cr3; mov cr3, rax;");
    }

    void cpu::mm_flush(void* addr) {
        asm volatile("invlpg [%0]" : : "r"(addr));
    }

    void cpu::mm_flush(void* addr, usz count) {
        for (usz off = 0; off < count; off++)
            cpu::mm_flush((void*) ((addr_t) addr + off * arch::mm::PG_SIZE));
    }

    /*void cpu::mm_flushbr(void* addr) {
        asm volatile("invlpgb" : : "a"(addr));
    }

    void cpu::mm_flushsync() {
        asm volatile("tlbsync");
    }*/

    void cpu::halt() {
        asm volatile("cli");
        asm volatile("hlt");
    }

    void cpu::dg_fault() {
        asm volatile("ud2");
    }

    void cpu::no_interrupts() {
        asm volatile("cli");
    }

    void cpu::interrupts() {
        asm volatile("sti");
    }

    bool cpu::check_interrupts() {
        usz flags;

        asm volatile("pushfq ; \
                    pop %0;"
                     : "=r"(flags)
                     :);

        return flags & 0x0200;
    }

    u8 cpu::inb(u16 m_port) {
        u8 val;
        asm volatile("inb %0, %1" : "=a"(val) : "dN"(m_port));
        return val;
    }

    u16 cpu::inw(u16 m_port) {
        u16 val;
        asm volatile("inw %0, %1" : "=a"(val) : "dN"(m_port));
        return val;
    }

    u32 cpu::ind(u16 m_port) {
        u32 val;
        asm volatile("ind %0, %1" : "=a"(val) : "d"(m_port));
        return val;
    }

    void cpu::outb(u16 m_port, u8 m_data) {
        asm volatile("outb %0, %1" : : "dN"(m_port), "a"(m_data));
    }

    void cpu::outw(u16 m_port, u16 m_data) {
        asm volatile("outw %0, %1" : : "dN"(m_port), "a"(m_data));
    }

    void cpu::outd(u16 m_port, u32 m_data) {
        asm volatile("outd %0, %1" : : "d"(m_port), "a"(m_data));
    }

    void cpu::use(mm::pagetbl* pagetbl, bool) {
        asm volatile("mov cr3, %0;" : : "r"(pagetbl->root()));
    }
}
