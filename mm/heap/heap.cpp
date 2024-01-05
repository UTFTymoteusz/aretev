#include "heap.hpp"

#include "art/core.hpp"
#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/mm.hpp"
#include "art/proc.hpp"
#include "art/string.hpp"

#include "afx/heap.h"

namespace art::mm::heap {
    afx::pool_t* main_pool; // General purpose heap pool.
    afx::pool_t* int_pool;  // Heap pool for interrupts.
    proc::lock   main_lock;
    proc::lock   int_lock;

    afx::pool_t* alloc_pool(usz size) {
        phys_t start = mm::ppalloc(size);
        void*  space = mm::kernel_pg.map(start, size, nullptr, PG_RW);

        return afx::heap_pool(space, size);
    }

    section(".boot.text") void init() {
        main_pool = alloc_pool(MPOOL_SIZE);
        int_pool  = alloc_pool(IPOOL_SIZE);
    }

    bool free_branch(void* mem, afx::pool_t* pool) {
        while (pool) {
            if (afx::heap_free(pool, mem))
                return true;

            pool = afx::heap_getnext(pool);
        }

        return false;
    }

    usz size_branch(void* mem, afx::pool_t* pool) {
        while (pool) {
            usz size = afx::heap_size(pool, mem);
            if (size)
                return size;

            pool = afx::heap_getnext(pool);
        }

        return false;
    }
}

namespace art::mm {
    void* malloc(usz size) {
        using namespace art::mm::heap;

        if (size >= arch::mm::PG_SIZE * 2) {
            ;
        }

        bool         interrupts = cpu::check_interrupts();
        afx::pool_t* current    = interrupts ? main_pool : int_pool;

        while (true) {
            void*       area = afx::heap_malloc(current, size);
            proc::lock& lock = interrupts ? main_lock : int_lock;

            if (area)
                return area;

            lock.acquire();

            if (!area && !afx::heap_getnext(current))
                afx::heap_setnext(current, alloc_pool(MPOOL_SIZE));

            lock.release();

            current = afx::heap_getnext(current);
        }

        return nullptr;
    }

    void free(void* memory) {
        using namespace art::mm::heap;

        if (!cpu::check_interrupts()) {
            if (!free_branch(memory, int_pool))
                free_branch(memory, main_pool);

            return;
        }

        if (!free_branch(memory, main_pool))
            free_branch(memory, int_pool);
    }

    usz msize(void* memory) {
        using namespace art::mm::heap;

        usz ssz;

        if ((ssz = size_branch(memory, main_pool)))
            return ssz;

        if ((ssz = size_branch(memory, int_pool)))
            return ssz;

        return 0;
    }

    void* realloc(void* ptr, usz size) {
        if (!ptr)
            return malloc(size);

        if (size == 0) {
            free(ptr);
            return nullptr;
        }

        usz   oldsize = msize(ptr);
        void* newptr  = malloc(size);

        if (!newptr) {
            free(ptr);
            return nullptr;
        }

        memcpy(newptr, ptr, min(oldsize, size));
        free(ptr);

        return newptr;
    }
}