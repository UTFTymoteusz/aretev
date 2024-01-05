#pragma once

#include "art/int.hpp"

namespace art {
    // Maximum amount of memory ranges.
    inline constexpr usz BOOT_MAX_MM_RANGES = 32;

    /**
     * @brief A struct that describes a framebuffer.
     */
    struct fb_info {
        u8* addr;
        u32 width, height;
        u32 bpp;
    };

    /**
     * @brief A struct that describes a device tree blob.
     */
    struct fdt_info {
        bool  present;
        void* pointer;
    };

    /**
     * @brief A struct that describes a memory mapping.
     */
    struct mm_range {
        addr_t addr;
        usz    len;
    };

    /**
     * @brief A struct that contains various information gathered while booting.
     */
    struct boot_info {
        fb_info  fb;
        fdt_info fdt;
        int      mm_range_count;
        mm_range mm_ranges[BOOT_MAX_MM_RANGES];
    };
}