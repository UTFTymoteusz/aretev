#include "sys/sys.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"

#include "dev/dev.hpp"
#include "sys/acpi/acpi.hpp"

namespace art::sys {
    ttydisp* display;
    device*  host;
    u8       acpirev;

    void init(boot_info*) {
        if ((display = art::arch::sys::create_display())) {
            display->clear().cursor(true).cursor(0, 0);
            dev::create_vttys();
        }

        host = art::arch::sys::create_host();

        acpi::init();
    }
}
