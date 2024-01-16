#pragma once

#include "art/int.hpp"

namespace art {
    inline const char* xtox_dictionary = "zyxwvutsrqponmlkjihgfedcba9876543210123456"
                                         "789abcdefghijklmnopqrstuvwxyz";

    // void snprintk(char* buffer, usz n, const char* format, ...);
    // void snprintk(char* buffer, usz n, const char* format, va_list args);

    template <typename T>
    char* ttos(T num, int base, char* buffer) {
        char* ptr;
        char* low;

        if (base < 2 || base > 36) {
            *buffer = '\0';
            return buffer;
        }

        ptr = buffer;
        if (num < 0 && base == 10)
            *ptr++ = '-';

        low = ptr;
        do {
            *ptr++ = xtox_dictionary[35 + num % base];
            num /= base;
        } while (num);

        *ptr-- = '\0';

        while (low < ptr) {
            char tmp = *low;

            *low++ = *ptr;
            *ptr-- = tmp;
        }
        return buffer;
    }

    template <typename T>
    T stot(int base, const char* str) {
        T   res  = 0;
        int sign = 1;
        int i    = 0;

        if (str[0] == '-') {
            sign = -1;
            ++i;
        }

        for (; str[i] != '\0'; ++i)
            res = res * base + str[i] - '0';

        return sign * res;
    }

    usz   strlen(const char* str);
    int   strcmp(const char* a, const char* b);
    usz   strlcpy(char* dst, const char* src, usz num);
    usz   strcspn(const char* str, const char* bongs);
    char* strndup(const char* str, usz len);
    usz   strspn(const char* str, const char* bongs);
    char* strntokp_r(char* buffer, usz len, const char* str, const char* delim,
                     const char** saveptr);
    char* strchr(const char* str, int c);
    char* strrchr(const char* str, int c);

    void* memset(void* mem, char c, usz len);
    void* memset16(void* mem, u16 val, usz count);
    void* memset32(void* mem, u32 val, usz count);
    void* memset64(void* mem, u64 val, usz count);
    void* memcpy(void* dst, const void* src, usz size);
    int   memcmp(const void* a, const void* b, usz num);

    template <typename T>
    void strtok_iterate(const char* str, const char* delim, T callback) {
        char        buffer[256];
        const char* ptr = nullptr;
        char*       token;

        while ((token =
                    strntokp_r(buffer, sizeof(buffer), ptr ? nullptr : str, delim, &ptr)))
            callback(token);
    }

    // inline char tolower(char c) {
    //     return inrange(c, 'A', 'Z') ? c + 32 : c;
    // }

    // inline char toupper(char c) {
    //     return inrange(c, 'a', 'z') ? c - 32 : c;
    // }
}