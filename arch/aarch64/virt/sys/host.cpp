#include "art/arch/sys.hpp"
#include "art/mm.hpp"

#include "arch/aarch64/virt/sys/host/aarch64virt.hpp"

namespace art::arch::sys {
    shptr<art::sys::devdesc> create_host() {
        auto host = art::mm::share<aarch64virt>();

        return host;
    }
}