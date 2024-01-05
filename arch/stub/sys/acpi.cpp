#include "art/data.hpp"
#include "art/sys.hpp"

namespace art::arch::sys::acpi {
    weak art::sys::acpi::rsdp* find_rsdp() {
        return nullptr;
    }

    weak art::sys::acpi::xsdp* find_xsdp() {
        return nullptr;
    }
}