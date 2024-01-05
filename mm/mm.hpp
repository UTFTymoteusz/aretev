#pragma once

#include "art/boot.hpp"

namespace art::mm {
    /**
     * @brief Initializes memory management.
     */
    void init(boot_info* info);
}