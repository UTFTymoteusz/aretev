#include "mm/phys/phys.hpp"

#include "art/arch/mm.hpp"
#include "art/boot.hpp"
#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

#include "mm/boot.hpp"

#define section_args(name) (name##_start, name##_end - name##_start)

namespace art::mm::phys {
    /**
     * @brief A bitmap that allows for 8 MiB of physical allocations.
     */
    section(".boot.data") aligned(16) u8 boot_bitmap[sizeof(bitmap) + 1024];

    bitmap* first_bitmap;

    /**
     * @brief Sets up the first_bitmap object.
     * @param info Boot info.
     */
    section(".boot.text") void setup_bootbitmap(boot_info* info) {
        u32 pages = alfloor(info->mm_ranges[0].len, arch::mm::PG_SIZE);

        first_bitmap = new ((void*) boot_bitmap) bitmap(info->mm_ranges[0].addr, pages);
        first_bitmap->next = nullptr;
    }

    /**
     * @brief Masks away frames taken up by the kernel as set.
     */
    section(".boot.text") void mask_kernel() {
        ppmask section_args(b_boot_text);
        ppmask section_args(b_boot_rodata);
        ppmask section_args(b_boot_data);
        ppmask section_args(b_boot_bss);
        ppmask section_args(b_kernel_text);
        ppmask section_args(b_kernel_rodata);
        ppmask section_args(b_kernel_data);
        ppmask section_args(b_kernel_bss);
        ppmask section_args(b_kernel_init);
    }

    section(".boot.text") void init(boot_info* info) {
        setup_bootbitmap(info);
        mask_kernel();
    }

    section(".boot.text") void migrate(boot_info* info) {
        bitmap* prev;

        for (int i = 0; i < info->mm_range_count; i++) {
            auto&   range = info->mm_ranges[i];
            u32     pages = alfloor(range.len, arch::mm::PG_SIZE);
            bitmap* map   = (bitmap*) malloc(sizeof(bitmap) + pages / 8);

            new (map) bitmap(range.addr, pages);

            if (i == 0) {
                memcpy(map->map, first_bitmap->map, 1024);
                first_bitmap = map;
            }
            else {
                prev->next = map;
            }

            prev = map;
        }
    }
}

namespace art::mm {
    void ppmask(phys_t paddr, usz len) {
        using namespace art::mm::phys;

        u32     pages   = alfit(len, arch::mm::PG_SIZE);
        bitmap* current = first_bitmap;

        while (current) {
            if (!current->inrange(paddr)) {
                current = current->next;
                continue;
            }

            current->setphys(paddr, pages);
            break;
        }
    }

    phys_t ppalloc(usz len) {
        using namespace art::mm::phys;

        u32     pages   = alfit(len, arch::mm::PG_SIZE);
        bitmap* current = first_bitmap;

        while (current) {
            u32 page = current->find(pages);

            if (page == 0xFFFFFFFF) {
                current = current->next;
                continue;
            }

            current->set(page, pages);

            return current->addr(page);
        }

        return 0x0000;
    }

    void ppfree(phys_t paddr, usz len) {
        using namespace art::mm::phys;

        u32     pages   = alfit(len, arch::mm::PG_SIZE);
        bitmap* current = first_bitmap;

        while (current) {
            if (!current->inrange(paddr)) {
                current = current->next;
                continue;
            }

            current->setphys(paddr, pages, false);
            break;
        }
    }
}