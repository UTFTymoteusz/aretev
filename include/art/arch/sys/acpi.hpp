#pragma once

#include "art/sys.hpp"

namespace art::arch::sys::acpi {
    art::sys::acpi::rsdp* find_rsdp();
    art::sys::acpi::xsdp* find_xsdp();
}