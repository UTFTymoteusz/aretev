#pragma once

#include "art/sys.hpp"

namespace art::arch::sys {
    art::sys::ttydisp*       create_display();
    shptr<art::sys::devdesc> create_host();
    void                     prepare_irq();
}