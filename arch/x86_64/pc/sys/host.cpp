#include "art/arch/sys.hpp"

#include "arch/x86_64/pc/sys/host/x86_64pc.hpp"

namespace art::arch::sys {
    art::sys::device* create_host() {
        art::sys::device* host = new x86_64pc();

        return host;
    }
}