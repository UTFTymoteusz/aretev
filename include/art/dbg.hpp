#pragma once

#include "art/int.hpp"

namespace art::dev {
    class device;
}

namespace art {
    extern dev::device* stdin;
    extern dev::device* stdout;
}

// A simple macro to simplify print-driven debugging.
#define marker       \
    if (art::stdout) \
        printk("%s:%u: marker\n", __FILE__, __LINE__);

namespace art::dbg {
    void memdump(void* addr, usz len);
}