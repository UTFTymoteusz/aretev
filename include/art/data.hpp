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

    template <typename T, typename U>
    constexpr bool find_if(T first, T last, U matcher) {
        for (T val = first; val < last; val++)
            if (matcher(*val))
                return true;

        return false;
    }

    template <typename T, typename U>
    constexpr bool find_if(T iterable, U matcher) {
        return find_if(iterable.begin(), iterable.end(), matcher);
    }

    template <typename T, typename U>
    constexpr bool find_if_not(T first, T last, U matcher) {
        for (T val = first; val < last; val++)
            if (!matcher(*val))
                return true;

        return false;
    }

    template <typename T, typename U>
    constexpr bool find_if_not(T iterable, U matcher) {
        return find_if_not(iterable.begin(), iterable.end(), matcher);
    }

    template <typename T, typename U>
    constexpr optional<T> find_first_match(T first, T last, U matcher) {
        for (T val = first; val < last; val++)
            if (matcher(*val))
                return val;

        return ENOENT;
    }
}