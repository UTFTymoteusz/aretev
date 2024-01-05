#pragma once

#include "art/int.hpp"

namespace art::mm::heap {
    constexpr usz MPOOL_SIZE = 786432; // Size of a main pool.
    constexpr usz IPOOL_SIZE = 262144; // Size of an interrupt pool.

    /**
     * @brief Initializes the heap.
     */
    void init();
}