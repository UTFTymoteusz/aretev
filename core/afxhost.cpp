#include "art/int.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

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

    extern "C" usz afxhost_strlen(const char* str) {
        return strlen(str);
    }

    extern "C" int afxhost_strcmp(const char* a, const char* b) {
        return strcmp(a, b);
    }

    extern "C" void* afxhost_memcpy(void* dst, const char* src, usz size) {
        return memcpy(dst, src, size);
    }
}