#include "art/sys.hpp"

#include "art/data.hpp"

namespace art::arch::sys {
    weak art::sys::ttydisp* create_display() {
        return nullptr;
    }

    weak void prepare_irq() {}
}