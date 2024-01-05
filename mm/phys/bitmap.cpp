#include "bitmap.hpp"

#include "art/arch/mm.hpp"
#include "art/cpu.hpp"
#include "art/dbg.hpp"
#include "art/int.hpp"
#include "art/string.hpp"

namespace art::mm::phys {
    constexpr int MM_PHYSBITS = sizeof(bits_t) * 8;

    bitmap::bitmap(phys_t a_start, u32 a_count) {
        this->start = a_start;
        this->count = a_count;
        this->free  = a_count;
        this->next  = nullptr;

        memset(map, '\0', a_count / MM_PHYSBITS);
    }

    void bitmap::set(u32 id, u32 count, bool taken) {
        u32 idi = id / MM_PHYSBITS; // Index in the map array.
        u16 idb = id % MM_PHYSBITS; // Bit index in a bits_t.

        while (count > 0) {
            int    bit_count = min<u32>(MM_PHYSBITS - idb, count);
            bits_t mask      = bitmask<bits_t>(bit_count) << idb;

            if (taken) {
                free -= bitc(~map[idi] & mask);
                map[idi] |= mask;
            }
            else {
                free += bitc(map[idi] & mask);
                map[idi] &= ~mask;
            }

            count -= min<u32>((32 - idb), count);
            idi++;
            idb = 0;
        }
    }

    void bitmap::setphys(phys_t paddr, u32 a_count, bool taken) {
        set((u32) ((paddr - start) / arch::mm::PG_SIZE), a_count, taken);
    }

    u32 bitmap::find(u32 a_count) {
        if (a_count == 0)
            return 0xFFFFFFFF;

        u32 starting = 0xFFFFFFFF;
        u32 found    = 0;

        for (u32 i = 0; i < count; i++) {
            if (map[i] == (bits_t) -1) {
                starting = 0xFFFFFFFF;
                found    = 0;

                continue;
            }
            else if (map[i] == (bits_t) 0) {
                if (starting == 0xFFFFFFFF)
                    starting = i * MM_PHYSBITS;

                found += MM_PHYSBITS;

                if (found >= a_count)
                    return starting;

                continue;
            }

            auto man = bitman<bits_t>(map[i]);

            for (u32 j = 0; j < MM_PHYSBITS; j++) {
                if (man.get(j)) {
                    starting = 0xFFFFFFFF;
                    found    = 0;

                    continue;
                }

                if (starting == 0xFFFFFFFF)
                    starting = i * MM_PHYSBITS + j;

                found++;

                if (found == a_count)
                    return starting;
            }
        }

        return starting;
    }

    usz bitmap::size() {
        return (usz) count * arch::mm::PG_SIZE;
    }

    bool bitmap::inrange(phys_t paddr) {
        return paddr >= start && paddr < start + size();
    }

    phys_t bitmap::addr(u32 id) {
        return start + id * arch::mm::PG_SIZE;
    }
}