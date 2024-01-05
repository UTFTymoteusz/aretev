#include "art/cpu.hpp"
#include "art/dbg.hpp"
#include "art/string.hpp"

namespace art::dbg {
    void memdump(void* addr, usz len) {
        /*u8* curr = (u8*) addr;
        u8* end  = (u8*) addr + len;
        int i    = 0;

        char bufferA[32] = {'0', 'x'};
        char bufferB[4]  = {'0', 'x'};

        for (; curr < end; curr++) {
            if (i == 0) {
                ttos((usz) curr, 16, bufferA + 2);

                for (int i = 0; bufferA[i]; i++)
                    cpu::outb(0x3F8, bufferA[i]);

                cpu::outb(0x3F8, ':');
                cpu::outb(0x3F8, ' ');
            }

            ttos(*curr, 16, bufferB);

            if (bufferB[1] == '\0') {
                bufferB[1] = bufferB[0];
                bufferB[0] = '0';
                bufferB[2] = '\0';
            }

            for (int i = 0; bufferB[i]; i++)
                cpu::outb(0x3F8, bufferB[i]);

            cpu::outb(0x3F8, ' ');

            if (i == 7) {
                cpu::outb(0x3F8, ' ');
                cpu::outb(0x3F8, ' ');
            }

            i++;

            if (i >= 16) {
                i = 0;

                cpu::outb(0x3F8, '\r');
                cpu::outb(0x3F8, '\n');
            }
        }*/
    }
}