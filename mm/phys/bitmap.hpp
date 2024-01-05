#pragma once

#include "art/int.hpp"

namespace art::mm::phys {
    typedef u32 bits_t;

    /**
     * @brief A bitmap of allocated physical pages.
     */
    class bitmap {
        public:
        phys_t  start; // The starting physical address that the bitmap describes.
        u32     count; // Total count of pages in the bitmap.
        u32     free;  // Count of free pages in the bitmap
        bitmap* next;  // Pointer to the next bitmap.
        bits_t  map[]; // Bitmap data.

        /**
         * @brief Constructs a new bitmap object.
         * @param addr  The starting physical address this bitmap describes.
         * @param count Amount of physical pages.
         */
        bitmap(phys_t addr, u32 count);

        /**
         * @brief Changes the status of physical pages.
         * @param id    Page ID to start at.
         * @param count Page count.
         * @param taken Bool to mark pages as taken.
         */
        void set(u32 id, u32 count, bool taken = true);

        /**
         * @brief Changes the status of physical pages. Takes a physical address instead
         * of an id.
         * @param id    Physical address to start at.
         * @param count Page count.
         * @param taken Bool to mark pages as taken.
         */
        void setphys(phys_t paddr, u32 count, bool taken = true);

        /**
         * @brief Finds a contiguous area of free pages.
         * @param count Page count.
         * @return Id of the starting page in the area, or 0xFFFFFFFF on failure.
         */
        u32 find(u32 count);

        /**
         * @brief Returns the size of the covered region in bytes
         * @return Bitmap total page size in bytes.
         */
        usz size();

        /**
         * @brief Checks whether paddr fits in the bitmap's covered region.
         * @return True if paddr is in the covered region.
         */
        bool inrange(phys_t paddr);

        /**
         * @brief Converts a page ID to a physical address.
         * @return Physical address of the page.
         */
        phys_t addr(u32 id);
    };
}