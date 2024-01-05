#include "art/dev.hpp"

namespace art {
    dev::device* stdin  = nullptr;
    dev::device* stdout = nullptr;

    void notify_ctty(dev::device* dev) {
        if ((dev->flags & dev::DV_TTY) != dev::DV_TTY)
            return;

        if (stdin == nullptr)
            stdin = dev;

        if (stdout == nullptr)
            stdout = dev;
    }
}