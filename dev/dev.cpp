#include "dev.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"
#include "art/sys.hpp"

#include "arch/x86_64/pc/dev/chr/16650/tts.hpp"
#include "dev/chr/tty/vtty.hpp"
#include "sys/sys.hpp"

namespace art::dev {
    void init(boot_info*) {}

    void create_vttys() {
        device* tty = new vtty("tty0", sys::display);
        register_device(tty);
    }

    error_t register_device(device* dev) {
        return ENONE;
    }
}