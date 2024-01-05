#include "art/sys/acpi/acpi.hpp"

#include "art/arch/sys.hpp"
#include "art/core.hpp"
#include "art/mm.hpp"

#include "sys/acpi/acpi.hpp"

namespace art::arch::sys::acpi {
    using namespace art;
    using namespace art::sys;
    using namespace art::sys::acpi;

    phys_t _find_rsdptr(usz start, usz length);

    phys_t find_rsdptr() {
        phys_t rsdptr;

        if ((rsdptr = _find_rsdptr(0x00, 1024)))
            return rsdptr;

        if ((rsdptr = _find_rsdptr(0x000E0000, 131071)))
            return rsdptr;

        return 0x0000;
    }

    phys_t _find_rsdptr(usz start, usz length) {
        u8* mem = (u8*) art::mm::kernel_pg.map(start, length, nullptr, art::mm::PG_READ);

        for (usz i = 0; i < length; i += 0x10) {
            u8* ptr = mem + i;

            if (memcmp(ptr, "RSD PTR ", 8) == 0) {
                art::mm::kernel_pg.unmap(mem, length);
                return start + i;
            }
        }

        art::mm::kernel_pg.unmap(mem, length);
        return 0x00;
    }

    art::sys::acpi::rsdp* find_rsdp() {
        phys_t rsd_ptr = find_rsdptr();
        if (!rsd_ptr)
            return nullptr;

        auto _rsdp = (rsdp*) art::mm::kernel_pg.map((phys_t) rsd_ptr, sizeof(rsdp),
                                                    nullptr, art::mm::PG_RW);
        auto _xsdp = (xsdp*) _rsdp;

        if (validate_table((void*) _xsdp, sizeof(xsdp)) &&
            _xsdp->xsdt_address != 0x0000) {
            art::mm::kernel_pg.unmap(_rsdp, sizeof(rsdp));
            return nullptr;
        }

        if (!validate_table((void*) _rsdp, sizeof(rsdp))) {
            art::mm::kernel_pg.unmap(_rsdp, sizeof(rsdp));
            return nullptr;
        }

        if (_rsdp->rsdt_address == 0x0000) {
            art::mm::kernel_pg.unmap(_rsdp, sizeof(rsdp));
            return nullptr;
        }

        return _rsdp;
    }

    art::sys::acpi::xsdp* find_xsdp() {
        phys_t rsd_ptr = find_rsdptr();
        if (!rsd_ptr)
            return nullptr;

        auto _xsdp = (xsdp*) art::mm::kernel_pg.map((phys_t) rsd_ptr, sizeof(xsdp),
                                                    nullptr, art::mm::PG_RW);

        if (!validate_table((void*) _xsdp, sizeof(xsdp))) {
            art::mm::kernel_pg.unmap(_xsdp, sizeof(xsdp));
            return nullptr;
        }

        if (_xsdp->xsdt_address == 0x0000) {
            art::mm::kernel_pg.unmap(_xsdp, sizeof(xsdp));
            return nullptr;
        }

        return _xsdp;
    }
}
