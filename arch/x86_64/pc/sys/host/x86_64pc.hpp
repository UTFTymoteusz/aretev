#pragma once

#include "art/sys.hpp"

namespace art::arch::sys {
    class x86_64pc : public art::sys::devdesc {
        public:
        x86_64pc();
        ~x86_64pc();

        private:
    };
}