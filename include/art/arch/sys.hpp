#pragma once

#include "art/sys.hpp"

namespace art::arch::sys {
    art::sys::ttydisp* create_display();
    art::sys::device*  create_host();
    void               prepare_irq();
}