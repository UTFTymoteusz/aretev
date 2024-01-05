#pragma once

#include "art/dev/device.hpp"

namespace art::dev {
    class netdev : public device {
        public:
        netdev(const char* name, int flags);
        virtual ~netdev();

        private:
    };
}
