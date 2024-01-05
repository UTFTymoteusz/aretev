#include "art/arch/sys.hpp"
#include "art/sys/ttydisp.hpp"

#include "arch/x86_64/pc/sys/egadisp.hpp"

namespace art::arch::sys {
    art::sys::ttydisp* create_display() {
        return new egadisp();
    }
}