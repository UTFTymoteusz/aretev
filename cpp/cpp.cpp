#include "art/int.hpp"
#include "art/mm.hpp"

using namespace art;
using namespace art::mm;

void* operator new(usz size) {
    return malloc(size);
}

void* operator new[](usz size) {
    return malloc(size);
}

void operator delete(void* ptr) {
    return free(ptr);
}

void operator delete[](void* ptr) {
    return free(ptr);
}

void operator delete(void* ptr, usz) {
    return free(ptr);
}

void operator delete[](void* ptr, usz) {
    return free(ptr);
}
