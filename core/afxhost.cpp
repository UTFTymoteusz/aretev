#include "art/int.hpp"
#include "art/mm.hpp"

namespace art {
    extern "C" void* afxhost_malloc(usz size) {
        return mm::malloc(size);
    }

    extern "C" void* afxhost_realloc(void* mem, usz size) {
        return mm::realloc(mem, size);
    }

    extern "C" void afxhost_free(void* mem) {
        mm::free(mem);
    }
}