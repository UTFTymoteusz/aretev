#include "art/arch/sys.hpp"

#include "arch/x86_64/pc/sys/host/x86_64pc.hpp"

namespace art::arch::sys {
    art::sys::devdesc* create_host() {
        art::sys::devdesc* host = new x86_64pc();

        return host;
    }
}