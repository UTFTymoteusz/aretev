#include "multiboot.h"

#include "art/boot.hpp"
#include "art/int.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

#include "boot/boot.hpp"

namespace art {
    section(".boot.text") void multiboot_parsefb(boot_info*        dstinfo,
                                                 multiboot_info_t* srcinfo) {
        dstinfo->fb.addr   = (u8*) srcinfo->framebuffer_addr;
        dstinfo->fb.width  = srcinfo->framebuffer_width;
        dstinfo->fb.height = srcinfo->framebuffer_height;
        dstinfo->fb.bpp    = srcinfo->framebuffer_bpp;
    }

    section(".boot.text") void multiboot_parsemm(boot_info*        dstinfo,
                                                 multiboot_info_t* srcinfo) {
        int  mmap_count = srcinfo->mmap_length / sizeof(multiboot_memory_map_t);
        auto mmap       = (multiboot_memory_map_t*) (usz) (srcinfo->mmap_addr);

        dstinfo->mm_range_count = 0;

        for (int i = 0; i < min<int>(mmap_count, BOOT_MAX_MM_RANGES); i++) {
            if (mmap[i].type != MULTIBOOT_MEMORY_AVAILABLE)
                continue;

            if (mmap[i].addr < mm::kernel_text_start)
                continue;

            int j = dstinfo->mm_range_count;

            dstinfo->mm_ranges[j].addr = mmap[i].addr;
            dstinfo->mm_ranges[j].len  = mmap[i].len;
            dstinfo->mm_range_count++;
        }
    }

    section(".boot.text") void multiboot_parse(boot_info*        dstinfo,
                                               multiboot_info_t* srcinfo) {
        memset(dstinfo, '\0', sizeof(boot_info));
        multiboot_parsefb(dstinfo, srcinfo);
        multiboot_parsemm(dstinfo, srcinfo);
    }

    extern "C" section(".boot.text") void multiboot_start(multiboot_info_t* minfo) {
        boot_info info;

        multiboot_parse(&info, minfo);
        start(&info);
    }
}