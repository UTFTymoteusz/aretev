#include "art/int.hpp"

#include "cpp/icxxabi.h"

using namespace art;

void* __dso_handle = 0;

usz               __atexit_func_count = 0;
atexit_func_entry __atexit_funcs[ATEXIT_MAX_FUNCS];

int __cxa_atexit(void (*f)(void*), void* objptr, void* dso) {
    if (__atexit_func_count >= ATEXIT_MAX_FUNCS)
        return -1;

    __atexit_funcs[__atexit_func_count].destructor_func = f;
    __atexit_funcs[__atexit_func_count].obj_ptr         = objptr;
    __atexit_funcs[__atexit_func_count].dso_handle      = dso;
    __atexit_func_count++;

    return 0;
};
