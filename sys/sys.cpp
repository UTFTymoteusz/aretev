#include "sys/sys.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"
#include "art/proc.hpp"

#include "afx/dt.h"
#include "dev/dev.hpp"
#include "drv/drivers.hpp"
#include "sys/acpi/acpi.hpp"

namespace art::sys {
    ttydisp*              display;
    devdesc*              host;
    u8                    acpirev;
    vector<sys::devdesc*> devdescs;
    vector<sys::driver*>  drivers;
    proc::lock            lock;

    void iterate_fdt(void* fdt);

    void init(boot_info* info) {
        if ((display = art::arch::sys::create_display())) {
            display->clear().cursor(true).cursor(0, 0);
            dev::create_vttys();
        }

        host = art::arch::sys::create_host();

        if (info->fdt.present)
            iterate_fdt(info->fdt.pointer);

        register_drivers();
        acpi::init();
    }

    void iterate_node(afx::fdt& parsed, afx::fdt_node& node) {
        afx::fdt_node child;
        afx::fdt_prop addr_cells;
        afx::fdt_prop size_cells;

        if (!afx::fdt_firstnode(&parsed, &child, &node))
            return;

        afx::fdt_getprop(&parsed, &addr_cells, &node, "#address-cells");
        afx::fdt_getprop(&parsed, &size_cells, &node, "#size-cells");

        int addr_len = afx::fdt_getu32(&parsed, &addr_cells, 0);
        int size_len = afx::fdt_getu32(&parsed, &size_cells, 0);

        do {
            afx::fdt_prop compatible;
            afx::fdt_prop reg;

            if (!afx::fdt_getprop(&parsed, &compatible, &child, "compatible"))
                continue;

            if (!afx::fdt_getprop(&parsed, &reg, &child, "reg"))
                continue;

            char* buffer = new char[compatible.size];
            memcpy(buffer, compatible.data, compatible.size);

            for (usz i = 0; i < compatible.size - 1; i++)
                buffer[i] = buffer[i] ?: ';';

            auto devd = new sys::devdesc(child.name);

            devd->bus    = "main";
            devd->parent = sys::host;
            devd->push(new sys::attribute("compatible", buffer, true));

            for (int i = 0; i < reg.size; i += addr_len * 4 + size_len * 4) {
                usz addr = addr_len == 1 ? afx::fdt_getu32(&parsed, &reg, i + 0)
                                         : afx::fdt_getu64(&parsed, &reg, i + 0);
                usz size = size_len == 1
                               ? afx::fdt_getu32(&parsed, &reg, i + addr_len * 4)
                               : afx::fdt_getu64(&parsed, &reg, i + addr_len * 4);

                devd->push(resource(RS_MEMORY, addr, addr + size));
            }

            sys::register_devdesc(devd);
        } while (afx::fdt_nextnode(&parsed, &child, &child));
    }

    void iterate_fdt(void* fdt) {
        afx::fdt      parsed;
        afx::fdt_node root;

        afx::fdt_parse(&parsed, fdt);
        afx::fdt_rootnode(&parsed, &root);

        iterate_node(parsed, root);
    }

    error_t register_devdesc(sys::devdesc* devd) {
        {
            proc::lock_guard lg(lock);
            devdescs.push(devd);
        }

        spawn_device(devd);

        return ENONE;
    }

    error_t register_driver(sys::driver* driver) {
        proc::lock_guard lg(lock);

        drivers.push(driver);

        for (auto devd : devdescs) {
            if (devd->owner)
                continue;

            if (driver->match(devd)) {
                auto result = driver->spawn(devd);
                if (result == ENONE) {
                    devd->owner = driver;
                    return result;
                }
            }
        }

        return ENONE;
    }

    error_t spawn_device(sys::devdesc* devd) {
        proc::lock_guard lg(lock);

        if (devd->owner)
            return EBUSY;

        for (auto driver : drivers) {
            if (driver->match(devd)) {
                auto result = driver->spawn(devd);
                if (result == ENONE) {
                    devd->owner = driver;
                    return result;
                }
            }
        }

        return ENOENT;
    }
}
