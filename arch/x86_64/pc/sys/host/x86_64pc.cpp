#include "x86_64pc.hpp"

#include "art/sys/types.hpp"

namespace art::arch::sys {
    using namespace art::sys;

    x86_64pc::x86_64pc() : devdesc("x86_64pc") {
        this->push(new attribute("arch", "x86_64"));
        this->push(new attribute("machine", "pc"));
    }

    x86_64pc::~x86_64pc() {}
}