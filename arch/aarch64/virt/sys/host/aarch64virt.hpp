#pragma once

#include "art/sys.hpp"

namespace art::arch::sys {
    class aarch64virt : public art::sys::device {
        public:
        aarch64virt();
        ~aarch64virt();

        private:
    };
}