#pragma once

#include "art/boot.hpp"

#include "sys/fdt.hpp"

namespace art::sys {
    void init(boot_info* info);
}