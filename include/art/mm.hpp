#pragma once

#include "art/arch/mm.hpp"
#include "art/core/shptr.hpp"
#include "art/int.hpp"
#include "art/mm/heap.hpp"
#include "art/mm/pagetbl.hpp"
#include "art/mm/phys.hpp"
#include "art/mm/segments.hpp"

inline void* operator new(art::usz, void* ptr) {
    return ptr;
}

namespace art::mm {
    // Starting address of the kernel's half of the address space.
    extern addr_t kernelhalf_start;
    // Ending address of the kernel's half of the address space.
    extern addr_t kernelhalf_end;

    // The base kernel pagetbl.
    extern pagetbl kernel_pg;

    /**
     * @brief Constructs a type and wraps it inside of a shptr.
     * @tparam T Type.
     * @tparam A Args.
     * @return A shared pointer of type T.
     */
    template <typename T, typename... A>
    shptr<T> share(A&&... args) {
        void* mem;

        do
            mem = mm::malloc(sizeof(T));
        while (!mem);

        T* object = new (mem) T(args...);
        return shptr<T>(object);
    }
}
