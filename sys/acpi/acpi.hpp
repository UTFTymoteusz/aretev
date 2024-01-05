#pragma once

#include "art/int.hpp"

namespace art::sys::acpi {
    /**
     * @brief Validates the checksum of a ACPI table.
     * @param tbl Pointer to the table.
     * @param len Length of the table.
     * @return Whether the table has the correct checksum.
     */
    bool validate_table(const void* tbl, usz len);

    /**
     * @brief Initializes ACPI.
     */
    void init();
}