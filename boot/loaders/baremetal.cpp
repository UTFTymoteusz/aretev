#include "art/boot.hpp"
#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/int.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

#include "afx/dt.h"
#include "boot/boot.hpp"

namespace art {
    extern "C" section(".boot.text") void baremetal_parse(boot_info* dstinfo, void* fdt) {
        afx::fdt      parsed;
        afx::fdt_node root;
        afx::fdt_node memory;
        afx::fdt_prop size_cells_pr;
        afx::fdt_prop addr_cells_pr;
        int           size_cells;
        int           addr_cells;

        memset(dstinfo, '\0', sizeof(boot_info));
        afx::fdt_parse(&parsed, fdt);
        afx::fdt_rootnode(&parsed, &root);
        afx::fdt_getprop(&parsed, &size_cells_pr, &root, "#size-cells");
        afx::fdt_getprop(&parsed, &addr_cells_pr, &root, "#address-cells");

        size_cells = afx::fdt_getu32(&parsed, &size_cells_pr);
        addr_cells = afx::fdt_getu32(&parsed, &size_cells_pr);

        if (afx::fdt_childnode(&parsed, &memory, &root, "memory", false)) {
            afx::fdt_prop reg;

            if (afx::fdt_getprop(&parsed, &reg, &memory, "reg")) {
                int stride = (size_cells + addr_cells) * sizeof(u32);
                int len    = reg.size;

                for (int i = 0; i < len / stride; i++) {
                    int addr_offset = i * stride;
                    int size_offset = i * stride + addr_cells * 4;

                    addr_t addr = addr_cells == 1
                                      ? afx::fdt_getu32(&parsed, &reg, addr_offset)
                                      : afx::fdt_getu64(&parsed, &reg, addr_offset);
                    usz    len  = size_cells == 1
                                      ? afx::fdt_getu32(&parsed, &reg, size_offset)
                                      : afx::fdt_getu64(&parsed, &reg, size_offset);

                    dstinfo->mm_range_count++;
                    dstinfo->mm_ranges[i].addr = addr;
                    dstinfo->mm_ranges[i].len  = len;
                }
            }
        }

        dstinfo->fdt.present = true;
        dstinfo->fdt.pointer = fdt;
    }

    extern "C" section(".boot.text") void baremetal_start(void* fdt) {
        boot_info info;

        baremetal_parse(&info, fdt);
        start(&info);
    }
}