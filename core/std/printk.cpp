#include "art/core/stdio.hpp"
#include "art/util.hpp"

#include <stdarg.h>

namespace art {
    enum printk_size {
        PK_SHORT  = 0,
        PK_NORMAL = 1,
        PK_LONG   = 2,
    };

    struct printk_info {
        bool        left_pad   = false;
        bool        force_sign = false;
        bool        prepend    = false;
        char        pad_char   = ' ';
        printk_size size       = PK_NORMAL;
    };

    template <typename T>
    static void stdout_flush(T& buffer) {
        if (buffer.left() == 0) {
            buffer.exhausted();
            return;
        }

        dev::iopkt packet = {};
        dev::iovec vec    = {
               .base = buffer.buffer,
               .len  = buffer.left(),
        };

        packet.cmd    = dev::IO_WRITE;
        packet.iov    = &vec;
        packet.iovcnt = 1;
        packet.len    = buffer.left();

        stdout->submit(&packet);

        while (!packet.completed)
            /*proc::thread::yield()*/;

        buffer.exhausted();
    }

    int printk(const char* fmt, ...) {
        // TODO: buffer size checks and proper flushes

        if (!stdout)
            return -ENODEV;

        if (strlen(fmt) == 0)
            return 0;

        buffer<char, 512> buffer;
        va_list           list;

        va_start(list, fmt);

        do {
            char c = *fmt;

            if (c != '%') {
                buffer.write(fmt, 1);
                continue;
            }

            fmt++;

            printk_info info;
            char        vbuffer[64];
            char*       bptr = vbuffer;

            vbuffer[0] = '\0';

            // %[flags][width][.precision][length]specifier
            // https://cplusplus.com/reference/cstdio/printf/

            switch (*fmt) {
            case 's': {
                const char* str = va_arg(list, const char*);

                if (!str)
                    str = "(null)";

                usz left = strlen(str);

                while (left) {
                    usz written = buffer.write(str, left);

                    str += written;
                    left -= written;

                    buffer.ready();
                    stdout_flush(buffer);
                }
            } break;
            case 'c': {
                char c = va_arg(list, int);

                bptr[0] = c;
                bptr[1] = '\0';
            } break;
            case 'd':
            case 'i': {
                int base;

                switch (*fmt) {
                case 'd':
                case 'i':
                    base = 10;
                    break;
                default:
                    base = 10;
                    break;
                }

                if (info.size == PK_SHORT)
                    ttos<s16>(va_arg(list, s32), base, bptr);
                else if (info.size == PK_NORMAL)
                    ttos<s32>(va_arg(list, s32), base, bptr);
                else if (info.size == PK_LONG)
                    ttos<s64>(va_arg(list, s64), base, bptr);
            } break;
            case 'u':
            case 'o':
            case 'x':
            case 'X':
            case 'p': {
                int base;

                switch (*fmt) {
                case 'o':
                    base = 8;
                    break;
                case 'u':
                    base = 10;
                    break;
                case 'x':
                case 'X':
                case 'p':
                    base = 16;
                    break;
                default:
                    base = 10;
                    break;
                }

                if (*fmt == 'p') {
                    info.size = PK_LONG;

                    memcpy(bptr, "0x", 2);
                    bptr += 2;
                }

                if (info.size == PK_SHORT)
                    ttos<u16>(va_arg(list, u32), base, bptr);
                else if (info.size == PK_NORMAL)
                    ttos<u32>(va_arg(list, u32), base, bptr);
                else if (info.size == PK_LONG)
                    ttos<u64>(va_arg(list, u64), base, bptr);
            } break;
            default:
                break;
            }

            if (vbuffer[0] != '\0')
                buffer.write(vbuffer, strlen(vbuffer));
        } while (*(++fmt) != '\0');

        va_end(list);

        buffer.ready();
        stdout_flush(buffer);

        return 0;
    }
}