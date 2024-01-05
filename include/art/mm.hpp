#pragma once

#include "art/arch/mm.hpp"
#include "art/int.hpp"
#include "art/mm/heap.hpp"
#include "art/mm/pagetbl.hpp"
#include "art/mm/phys.hpp"
#include "art/mm/segments.hpp"

namespace art::mm {
    // Starting address of the kernel's half of the address space.
    extern addr_t kernelhalf_start;
    // Ending address of the kernel's half of the address space.
    extern addr_t kernelhalf_end;

    // The base kernel pagetbl.
    extern pagetbl kernel_pg;
}
