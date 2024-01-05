#pragma once

#include "art/boot.hpp"

#include "mm/phys/bitmap.hpp"

namespace art::mm::phys {
    /**
     * @brief Initializes the physical page allocator.
     * @param info Boot info.
     */
    void init(boot_info* info);

    /**
     * @brief Migrates the initial bitmap to the heap, and indexes the rest of the memory.
     * @param info Boot info.
     */
    void migrate(boot_info* info);
}