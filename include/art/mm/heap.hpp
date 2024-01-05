#pragma once

#include "art/int.hpp"

namespace art::mm {
    void* malloc(usz len);
    void  free(void* ptr);
    void* realloc(void* ptr, usz size);

    usz msize(void* ptr);
}