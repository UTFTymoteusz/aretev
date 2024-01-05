#pragma once

#include "art/data.hpp"
#include "art/int.hpp"

namespace art::arch::proc {
    packed struct fpu_context {
        u16 fcw;
        u16 fsw;
        u8  ftw;
        u8  reserved0;
        u16 fop;
        u32 fip;
        u16 fcs;
        u16 reserved1;

        u32 fdp;
        u16 fds;
        u16 reserved2;
        u32 mxcsr = 0x1F80;
        u32 mxcsr_mask;

        union {
            u8 st0[16];
            u8 mm0[16];
        };
        union {
            u8 st1[16];
            u8 mm1[16];
        };
        union {
            u8 st2[16];
            u8 mm2[16];
        };
        union {
            u8 st3[16];
            u8 mm3[16];
        };
        union {
            u8 st4[16];
            u8 mm4[16];
        };
        union {
            u8 st5[16];
            u8 mm5[16];
        };
        union {
            u8 st6[16];
            u8 mm6[16];
        };
        union {
            u8 st7[16];
            u8 mm7[16];
        };

        u8 xmm0[16];
        u8 xmm1[16];
        u8 xmm2[16];
        u8 xmm3[16];
        u8 xmm4[16];
        u8 xmm5[16];
        u8 xmm6[16];
        u8 xmm7[16];
        u8 xmm8[16];
        u8 xmm9[16];
        u8 xmm10[16];
        u8 xmm11[16];
        u8 xmm12[16];
        u8 xmm13[16];
        u8 xmm14[16];
        u8 xmm15[16];

        u8 reserved3[48];
        u8 available[48];
    };

    packed struct context {
        u64 r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;
        u64 cr3;

        u64 padding;
        u64 rip, cs, rflags, rsp, ss;

        fpu_context fxstate;
    };
}