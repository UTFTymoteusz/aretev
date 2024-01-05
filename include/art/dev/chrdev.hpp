#pragma once

#include "art/dev/device.hpp"

namespace art::dev {
    class chrdev : public device {
        public:
        chrdev(const char* name, int flags);
        virtual ~chrdev();

        virtual error_t ioctl(unsigned long request, void* argp);

        private:
    };
}
