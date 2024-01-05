#pragma once

#include "art/boot.hpp"

#include "mm/phys/bitmap.hpp"

namespace art::mm {
    /**
     * @brief Masks away physical pages.
     * @param paddr Physical address.
     * @param len   Length of allocation in bytes.
     */
    void ppmask(phys_t paddr, usz len);

    /**
     * @brief Allocates physical pages.
     * @param len Length of allocation in bytes.
     */
    phys_t ppalloc(usz len);

    /**
     * @brief Frees physical pages.
     * @param paddr Physical address of allocation.
     * @param len   Length of allocation in bytes.
     */
    void ppfree(phys_t paddr, usz len);
}