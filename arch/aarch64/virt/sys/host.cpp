#include "art/arch/sys.hpp"

#include "arch/aarch64/virt/sys/host/aarch64virt.hpp"

namespace art::arch::sys {
    art::sys::devdesc* create_host() {
        art::sys::devdesc* host = new aarch64virt();

        return host;
    }
}