#pragma once

#include "art/boot.hpp"

namespace art::dev {
    void init(boot_info* info);
    void create_vttys();
}