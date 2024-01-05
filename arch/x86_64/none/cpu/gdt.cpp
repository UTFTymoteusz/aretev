#include "gdt.hpp"

namespace art::arch {
    void gdt_segment::null() {
        limit0   = 0;
        base0    = 0;
        base1    = 0;
        access   = 0;
        limit1   = 0;
        base2    = 0;
        reserved = 0;
    }
}
