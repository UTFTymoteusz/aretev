#include "art/dev/chrdev.hpp"

namespace art::dev {
    chrdev::chrdev(const char* name, int flags) : device(name, flags | DV_CHAR) {}
    chrdev::~chrdev() {}

    error_t chrdev::ioctl(unsigned long, void*) {
        return EINVAL;
    }
}