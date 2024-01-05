#include "art/mm/pagetbl.hpp"

#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/dbg.hpp"
#include "art/int.hpp"
#include "art/string.hpp"

extern "C" void* pml4t;

namespace art::mm {
    void print_pg(const char* id, phys_t val) {
        cpu::outb(0x3F8, '[');

        for (int i = 0; id[i]; i++)
            cpu::outb(0x3F8, id[i]);

        cpu::outb(0x3F8, ']');
        cpu::outb(0x3F8, ' ');

        char buffer[48] = {'0', 'x'};

        ttos(val, 16, buffer + 2);

        for (int i = 0; buffer[i]; i++)
            cpu::outb(0x3F8, buffer[i]);

        cpu::outb(0x3F8, '\r');
        cpu::outb(0x3F8, '\n');
    }
}