#include "dev.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"
#include "art/sys.hpp"

#include "arch/x86_64/pc/dev/chr/16650/tts.hpp"
#include "dev/chr/tty/vtty.hpp"
#include "sys/sys.hpp"

namespace art::dev {
    void iterate_fdt(void* fdt);

    void init(boot_info* info) {
        if (info->fdt.present)
            iterate_fdt(info->fdt.pointer);
    }

    void create_vttys() {
        device* tty = new vtty("tty0", sys::display);
        register_device(tty);
    }

    error_t register_device(device* dev) {
        return ENONE;
    }

    void iterate_fdt(void* fdt) {
        sys::fdt      parsed;
        sys::fdt_node root;
        sys::fdt_node node;

        sys::fdt_parse(&parsed, fdt);
        sys::fdt_rootnode(&parsed, &root);

        if (!sys::fdt_firstnode(&parsed, &node, &root))
            return;

        do {
            node = node;
            //
        } while (sys::fdt_nextnode(&parsed, &node, &node));
    }
}