#include "sys/sys.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"
#include "art/proc.hpp"

#include "afx/dt.h"
#include "dev/dev.hpp"
#include "drv/drivers.hpp"
#include "sys/acpi/acpi.hpp"

namespace art::sys {
    struct phandle_assoc {
        u32            phandle;
        shptr<devdesc> devd;

        phandle_assoc(u32 phandle, const shptr<devdesc>& devd) : devd(devd) {
            this->phandle = phandle;
        }
    };

    struct phandle_link {
        shptr<devdesc> devd;
        u32            phandle;
        char           name[32];

        phandle_link(const shptr<devdesc>& devd, u32 phandle, const char* name)
            : devd(devd) {
            this->phandle = phandle;
            strlcpy(this->name, name, sizeof(name));
        }
    };

    ttydisp*                    display;
    shptr<devdesc>              host;
    u8                          acpirev;
    vector<shptr<sys::devdesc>> devdescs;
    vector<shptr<sys::driver>>  drivers;
    proc::lock                  lock;

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

    optional<shptr<devdesc>> prepare_devd(afx::fdt& parsed, afx::fdt_node& node,
                                          usz addr_len, usz size_len,
                                          vector<phandle_assoc>& phandles,
                                          vector<phandle_link>&  links) {
        afx::fdt_prop compatible;
        afx::fdt_prop reg;

        if (!afx::fdt_getprop(&parsed, &compatible, &node, "compatible"))
            return ENOENT;

        auto devd = mm::share<sys::devdesc>(node.name);

        char* buffer = new char[compatible.size];
        memcpy(buffer, compatible.data, compatible.size);

        for (usz i = 0; i < compatible.size - 1; i++)
            buffer[i] = buffer[i] ?: ';';

        devd->push(new sys::attribute("compatible", buffer, true));

        if (afx::fdt_getprop(&parsed, &reg, &node, "reg")) {
            for (usz i = 0; i < reg.size; i += addr_len * 4 + size_len * 4) {
                usz addr = addr_len == 1 ? afx::fdt_getu32(&parsed, &reg, i + 0)
                                         : afx::fdt_getu64(&parsed, &reg, i + 0);
                usz size = size_len == 1
                               ? afx::fdt_getu32(&parsed, &reg, i + addr_len * 4)
                               : afx::fdt_getu64(&parsed, &reg, i + addr_len * 4);

                devd->push(resource(RS_MEMORY, addr, addr + size));
            }
        }

        afx::fdt_prop phandle;

        if (afx::fdt_getprop(&parsed, &phandle, &node, "phandle")) {
            u32 id = afx::fdt_getu32(&parsed, &phandle, 0);
            phandles.emplace(id, devd);
        }

        afx::fdt_prop clocks;

        if (afx::fdt_getprop(&parsed, &clocks, &node, "clocks")) {
            int clock_count = clocks.size / sizeof(u32);

            for (int i = 0; i < clock_count; i++) {
                // TODO: proper snprintf
                u32  id         = afx::fdt_getu32(&parsed, &phandle, i * sizeof(u32));
                char buffer[32] = {'c', 'l', 'o', 'c', 'k', (char) ('0' + i)};

                links.emplace(devd, id, buffer);
            }
        }

        return devd;
    }

    void iterate_node(afx::fdt& parsed, afx::fdt_node node,
                      vector<phandle_assoc>& phandles, vector<phandle_link>& links) {
        afx::fdt_node child;
        afx::fdt_prop addr_cells;
        afx::fdt_prop size_cells;

        if (!afx::fdt_firstnode(&parsed, &child, &node))
            return;

        bool has_addr_len =
            afx::fdt_getprop(&parsed, &addr_cells, &node, "#address-cells");
        bool has_size_len = afx::fdt_getprop(&parsed, &size_cells, &node, "#size-cells");

        int addr_len = has_addr_len ? afx::fdt_getu32(&parsed, &addr_cells, 0) : 1;
        int size_len = has_size_len ? afx::fdt_getu32(&parsed, &size_cells, 0) : 1;

        do {
            auto devd_try =
                prepare_devd(parsed, child, addr_len, size_len, phandles, links);

            if (devd_try) {
                auto devd = devd_try.value;

                devd->bus    = "main";
                devd->parent = sys::host;

                sys::register_devdesc(devd);
            }

            iterate_node(parsed, child, phandles, links);
        } while (afx::fdt_nextnode(&parsed, &child, &child));
    }

    void link(vector<phandle_assoc>& phandles, vector<phandle_link>& links) {
        for (auto link : links) {
            auto matched = find_first_match(
                phandles.begin(), phandles.end(),
                [&link](phandle_assoc assoc) { return assoc.phandle == link.phandle; });

            link.devd->push(new attribute(link.name, matched.value->devd));
        }
    }

    void iterate_fdt(void* fdt) {
        afx::fdt              parsed;
        afx::fdt_node         root;
        vector<phandle_assoc> phandles;
        vector<phandle_link>  links;

        afx::fdt_parse(&parsed, fdt);
        afx::fdt_rootnode(&parsed, &root);

        iterate_node(parsed, root, phandles, links);
        link(phandles, links);
    }

    error_t register_devdesc(shptr<sys::devdesc> devd) {
        {
            proc::lock_guard lg(lock);
            devdescs.push(devd);
        }

        spawn_device(devd);

        return ENONE;
    }

    error_t register_driver(shptr<sys::driver> driver) {
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

    error_t spawn_device(shptr<sys::devdesc> devd) {
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
