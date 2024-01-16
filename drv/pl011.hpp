#pragma once

#include "art/dev.hpp"
#include "art/sys.hpp"

namespace art {
    class pl011drv : public sys::driver {
        public:
        pl011drv();

        error_t spawn(sys::devdesc* dev);
    };

    class pl011 : dev::chrdev {
        public:
        private:
    };
}