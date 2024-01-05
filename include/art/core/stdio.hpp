#pragma once

#include "art/dev.hpp"

namespace art {
    extern dev::device* stdin;
    extern dev::device* stdout;

    /**
     * @brief Notifies the kernel stdio about a possible new controlling terminal.
     * @param dev The device.
     */
    void notify_ctty(dev::device* dev);

    /**
     * @brief Writes out a format string to the stdout.
     * Behaves almost like the stdio printf().
     * @param fmt Format string.
     * @param ... Arguments to format with.
     * @return Amount of characters written.
     */
    int printk(const char* fmt, ...);
}