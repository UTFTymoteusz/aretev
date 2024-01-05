#pragma once

#include "art/errno.hpp"

#define packed __attribute__((packed))
#define weak __attribute__((weak))
#define aligned(x) __attribute__((aligned(x)))
#define section(s) __attribute__((section(s)))

#define ensure(v, func) \
    if (!func(v))       \
        return;

namespace art {
    template <typename T>
    struct optional {
        bool    has_value = false;
        error_t error     = ENONE;
        T       value;

        optional() {}

        optional(T value) : value(value) {
            has_value = true;
        }

        optional(error_t a_error) {
            error = a_error;
        }

        optional(T value, error_t a_error) : value(value) {
            has_value = true;
            error     = a_error;
        }

        operator bool() {
            return has_value;
        }

        operator error_t() {
            return error;
        }

        operator T() {
            return value;
        }
    };
}