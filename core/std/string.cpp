#include "art/string.hpp"

namespace art {
    usz strlen(const char* str) {
        usz len = 0;

        while (*str++ != '\0')
            len++;

        return len;
    }

    int strcmp(const char* a, const char* b) {
        do {
            if (*a < *b)
                return -1;
            else if (*a > *b)
                return 1;
        } while (*a++ != '\0' && *b++ != '\0');

        return 0;
    }

    usz strlcpy(char* dst, const char* src, usz num) {
        usz src_len = strlen(src);

        if (num == 0)
            return src_len;

        usz len = min(src_len, num - 1);
        memcpy(dst, src, len);

        dst[len] = '\0';

        return src_len;
    }

    char* strndup(const char* str, usz len) {
        char* nstr = new char[len + 1];
        strlcpy(nstr, str, len + 1);

        return nstr;
    }

    usz strspn(const char* str, const char* bongs) {
        usz n    = 0;
        usz cmps = strlen(bongs);

        while (str[n] != '\0') {
            bool safe = false;

            for (usz i = 0; i < cmps; i++)
                if (str[n] == bongs[i]) {
                    safe = true;
                    break;
                }

            if (!safe)
                break;

            n++;
        }

        return n;
    }

    usz strcspn(const char* str, const char* bongs) {
        usz n    = 0;
        usz cmps = strlen(bongs);

        while (str[n] != '\0') {
            for (usz i = 0; i < cmps; i++)
                if (str[n] == bongs[i])
                    return n;

            n++;
        }

        return n;
    }

    char* strntokp_r(char* buffer, usz len, const char* str, const char* delim,
                     const char** saveptr) {
        if (str) {
            usz len = strspn(str, delim);
            if (len == 0)
                if (*str == '\0')
                    return nullptr;

            *saveptr = str + len;
        }

        usz tok_len = strcspn(*saveptr, delim);
        if (tok_len == 0)
            return nullptr;

        usz copy_len = min(len, tok_len);

        if (copy_len < len)
            buffer[copy_len] = '\0';

        memcpy(buffer, *saveptr, copy_len);
        *saveptr += tok_len;

        usz delim_len = strspn(*saveptr, delim);
        *saveptr += delim_len;

        return buffer;
    }

    char* strchr(const char* str, int c) {
        usz len = strlen(str);

        for (usz i = 0; i < len; i++)
            if (str[i] == c)
                return (char*) &str[i];

        return nullptr;
    }

    char* strrchr(const char* str, int c) {
        usz len = strlen(str);

        for (usz i = len - 1; i > 0; i--)
            if (str[i] == c)
                return (char*) &str[i];

        return nullptr;
    }

    void* memset(void* mem, char c, usz len) {
        char* m_mem = (char*) mem;

        for (usz i = 0; i < len; i++)
            m_mem[i] = c;

        return mem;
    }

    void* memset16(void* mem, u16 val, usz count) {
        u16* m_mem = (u16*) mem;

        for (usz i = 0; i < count; i++)
            m_mem[i] = val;

        return mem;
    }

    void* memset32(void* mem, u32 val, usz count) {
        u32* m_mem = (u32*) mem;

        for (usz i = 0; i < count; i++)
            m_mem[i] = val;

        return mem;
    }

    void* memset64(void* mem, u64 val, usz count) {
        u64* m_mem = (u64*) mem;

        for (usz i = 0; i < count; i++)
            m_mem[i] = val;

        return mem;
    }

    void* memcpy(void* dst, const void* src, usz size) {
        u8* dst_b = (u8*) dst;
        u8* src_b = (u8*) src;

        for (usz i = 0; i < size; i++)
            dst_b[i] = src_b[i];

        return dst;
    }

    int memcmp(const void* a, const void* b, usz num) {
        u8* _a = (u8*) a;
        u8* _b = (u8*) b;

        for (usz i = 0; i < num; i++)
            if (_a[i] < _b[i])
                return -1;
            else if (_a[i] > _b[i])
                return 1;

        return 0;
    }
}