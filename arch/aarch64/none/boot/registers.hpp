#include "art/int.hpp"

namespace art {
    enum tcr_el1f {
        TCR_GRAN4K    = 0b10,
        TCR_GRAN64K   = 0b11,
        TCR_GRAN16K   = 0b01,
        TCR_SHNONE    = 0b00,
        TCR_SHOUTER   = 0b01,
        TCR_SHINNER   = 0b11UL,
        TCR_CHNONE    = 0b00,
        TCR_CHWBRAWA  = 0b01,
        TCR_CHWTRANWA = 0b10,
        TCR_CHWBRANWA = 0b11UL,
    };

    struct tcr_el1 {
        /*
         * TBI1[38]
         * TBI0[37]
         * AS[36]
         * IPS[34:32]
         * TG1[31:30]
         * SH1[29:28]
         * ORGN1[27:26]
         * IRGN1[25:24]
         * EPD1[23]
         * A1[22]
         * T1SZ[21:16]
         * TG0[15:14]
         * SH0[13:12]
         * ORGN0[11:10]
         * IRGN0[9:8]
         * EPD0[7]
         * T0SZ[5:0]
         */
        u64 value = 0;

        // Physical address size bits.
        tcr_el1& ips(u8 size_or_attribute);

        // Upper half top byte ignore.
        tcr_el1& tbi1(bool ignore);

        // Lower half top byte ignore.
        tcr_el1& tbi0(bool ignore);

        // Upper half granularity.
        tcr_el1& tg1(u8 attribute);

        // Upper half shareability attribute.
        tcr_el1& sh1(u8 attribute);

        // Upper half outer cacheability attribute.
        tcr_el1& orgn1(u8 attribute);

        // Upper half inner cacheability attribute.
        tcr_el1& irgn1(u8 attribute);

        // Upper half paging enable.
        tcr_el1& epd1(bool enabled);
        // Upper paging size.
        tcr_el1& t1sz(int sz);

        // Lower half granularity.
        tcr_el1& tg0(u8 attribute);

        // Lower half shareability attribute.
        tcr_el1& sh0(u8 attribute);

        // Lower half outer cacheability attribute.
        tcr_el1& orgn0(u8 attribute);

        // Lower half inner cacheability attribute.
        tcr_el1& irgn0(u8 attribute);

        // Lower half paging enable.
        tcr_el1& epd0(bool enabled);

        // Lower half paging size.
        tcr_el1& t0sz(int sz);
    };

    struct sctlr_el1 {
        /*
         * EE[25]
         * E0E[24]
         * WXN[19]
         * I[12]
         * UMA[9]
         * SA0[4]
         * SA[3]
         * C[2]
         * A[1]
         * M[0]
         */
        u64 value = 0;

        // Big endian accesses at EL1 enable.
        sctlr_el1& ee(bool big_endian);

        // Big endian accesses at EL0 enable.
        sctlr_el1& e0e(bool big_endian);

        // Treat all writeable regions as NX enable.
        sctlr_el1& wxn(bool enabled);

        // Instruction access cache enable.
        sctlr_el1& i(bool enabled);

        // User mask access.
        sctlr_el1& uma(bool enabled);

        // SP alignment check for EL0 enable.
        sctlr_el1& sa0(bool enabled);

        // SP alignment check enable.
        sctlr_el1& sa(bool enabled);

        // Cacheability enable.
        sctlr_el1& c(bool enabled);

        // Alignment check enable.
        sctlr_el1& a(bool enabled);

        // Memory management unit enable.
        sctlr_el1& m(bool enabled);
    };
}