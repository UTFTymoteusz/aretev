#pragma once

#include "art/int.hpp"

namespace art::arch::mm {
    constexpr addr_t PG_SIZE   = 4096;           // Page size.
    constexpr addr_t PG_MASK   = ~(PG_SIZE - 1); // Mask that filters out any flags.
    constexpr addr_t PG_OFFSET = 12;             // How many bits are taken up by flags.
    constexpr addr_t PG_SHIFT  = 9;              // How many bits are taken up by a level.
    constexpr addr_t PG_LEVELS = 4;              // Table level count.
    constexpr u16    PG_LO  = 500; // Index that points to the the top level paging table.
    constexpr u16    PG_LOB = 499; // Index used for migrating page trees.

    constexpr addr_t PG_LOADDR = 0xFFFFul << 48 | (addr_t) PG_LO << 39 |
                                 (addr_t) PG_LO << 30 | (addr_t) PG_LO << 21 |
                                 (addr_t) PG_LO << 12;
    constexpr addr_t PG_LOBADDR = 0xFFFFul << 48 | (addr_t) PG_LOB << 39 |
                                  (addr_t) PG_LOB << 30 | (addr_t) PG_LOB << 21 |
                                  (addr_t) PG_LOB << 12;

    /**
     * @brief Bits of each architectural page flag.
     */
    enum pgabits {
        PN_PRESENT = (1 << 0),
        PN_RO      = (0 << 1),
        PN_RW      = (1 << 1),
        PN_USER    = (1 << 2),
    };
}