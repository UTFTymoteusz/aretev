#pragma once

#include "art/data.hpp"
#include "art/int.hpp"

namespace art::arch {
    struct packed gdt_segment {
        u16 limit0;
        u16 base0;
        u8  base1;
        u8  access;
        u16 limit1;
        u32 base2;
        u32 reserved;

        /**
         * @brief Initializes the segment as a null segment.
         */
        void null();
    };

    /**
     * @brief Loads the specified Global Descriptor Table.
     * @param gdt Pointer to the GDT.
     * @param len Count of GDT segments.
     */
    void load_gdt(gdt_segment* gdt, int len);

    static_assert(sizeof(gdt_segment) == 16);
}
