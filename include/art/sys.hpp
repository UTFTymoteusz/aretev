#pragma once

#include "art/sys/acpi/acpi.hpp"
#include "art/sys/devdesc.hpp"
#include "art/sys/driver.hpp"
#include "art/sys/ttydisp.hpp"
#include "art/sys/types.hpp"

namespace art::sys {
    extern ttydisp* display; // The ttydisp that represents the framebuffer of the host.
    extern devdesc* host;    // The device that represents the local host.
    extern u8       acpirev; // The revision of the host's ACPI.

    /**
     * @brief Gets a discovered ACPI table.
     * This function accepts an index argument in order to accomodate multiple tables with
     * the same signature.
     * @param signature ACPI signature of the table.
     * @param index     Index of the table.
     * @return The table or nullptr.
     */
    acpi::table* get_table(const char signature[4], int index);

    error_t register_devdesc(sys::devdesc* devd);
    error_t register_driver(sys::driver* drv);
    error_t spawn_device(sys::devdesc* dev);
}