#include "aarch64virt.hpp"

#include "art/sys/types.hpp"

namespace art::arch::sys {
    using namespace art::sys;

    aarch64virt::aarch64virt() : devdesc("aarch64virt") {
        this->push(new attribute("arch", "aarch64"));
        this->push(new attribute("machine", "virt"));
    }

    aarch64virt::~aarch64virt() {}
}