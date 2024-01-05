#include "art/arch/sys.hpp"

#include "arch/aarch64/virt/sys/host/aarch64virt.hpp"

namespace art::arch::sys {
    art::sys::device* create_host() {
        art::sys::device* host = new aarch64virt();

        return host;
    }
}