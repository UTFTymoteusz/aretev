#pragma once

#include "art/dev/blkdev.hpp"
#include "art/dev/chr/tty.hpp"
#include "art/dev/chrdev.hpp"
#include "art/dev/device.hpp"
#include "art/dev/ioqueue.hpp"
#include "art/dev/iovecman.hpp"
#include "art/dev/netdev.hpp"
#include "art/dev/types.hpp"

namespace art::dev {
    error_t register_device(dev::device* dev);
}