#pragma once

#include "art/int.hpp"

// #define ART_ARCH_PG_DEFAULT_RO
#define ART_ARCH_PG_DEFAULT_RW
#define ART_ARCH_PG_NEEDS_ACCESS

namespace art::arch::mm {
    inline constexpr addr_t PG_SIZE = 4096;           // Page size.
    inline constexpr addr_t PG_MASK = ~(PG_SIZE - 1); // Mask that filters out any flags.
    inline constexpr addr_t PG_OFFSET = 12; // How many bits are taken up by flags.
    inline constexpr addr_t PG_SHIFT  = 9;  // How many bits are taken up by a level.
    inline constexpr addr_t PG_LEVELS = 3;  // Table level count.
    inline constexpr u16    PG_LO =
        500; // Index that points to the the top level paging table.
    inline constexpr u16 PG_LOB = 499; // Index used for migrating page trees.

    inline constexpr addr_t PG_LOADDR = 0xFFFFul << 48 | (addr_t) 511 << 39 |
                                        (addr_t) PG_LO << 30 | (addr_t) PG_LO << 21 |
                                        (addr_t) PG_LO << 12;
    inline constexpr addr_t PG_LOBADDR = 0xFFFFul << 48 | (addr_t) 511 << 39 |
                                         (addr_t) PG_LOB << 30 | (addr_t) PG_LOB << 21 |
                                         (addr_t) PG_LOB << 12;

    /**
     * @brief Bits of each architectural page flag.
     */
    enum pgabits {
        PN_PRESENT = (1 << 10) | (1 << 1) | (1 << 0),
        PN_RO      = (0b10 << 6),
        PN_RW      = (0b00 << 6),
        PN_USER    = (0b01 << 6),
        PN_NOCACHE = (0b10 << 8),
    };
}