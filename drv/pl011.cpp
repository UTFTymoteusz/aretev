#include "drv/pl011.hpp"

namespace art {
    pl011drv::pl011drv() : sys::driver("pl011") {}

    error_t pl011drv::spawn(sys::devdesc* dev) {
        return ENOSYS;
    }
}