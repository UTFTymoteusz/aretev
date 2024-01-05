#pragma once

#include "art/data.hpp"
#include "art/sys/ttydisp.hpp"

namespace art::arch::sys {
    enum egacolor {
        EGA_BLACK    = 0,
        EGA_BLUE     = 1,
        EGA_GREEN    = 2,
        EGA_CYAN     = 3,
        EGA_RED      = 4,
        EGA_MAGENTA  = 5,
        EGA_BROWN    = 6,
        EGA_WHITE    = 7,
        EGA_GRAY     = 8,
        EGA_BBLUE    = 9,
        EGA_BGREEN   = 10,
        EGA_BCYAN    = 11,
        EGA_BRED     = 12,
        EGA_BMAGENTA = 13,
        EGA_YELLOW   = 14,
        EGA_BWHITE   = 15,
    };

    struct packed egachar {
        char c;
        u8   color;
    };

    class egadisp : public art::sys::ttydisp {
        public:
        egadisp();
        ~egadisp();

        egadisp& write(int x, int y, const char* str, art::sys::ttycolor_t bg,
                       art::sys::ttycolor_t fg, usz len);
        egadisp& clear(char c, art::sys::ttycolor_t bg, art::sys::ttycolor_t fg);

        egadisp& cursor(bool enabled);
        egadisp& cursor(int x, int y);

        art::sys::ttycolor_t forecolor(art::sys::ansicolor color);
        art::sys::ttycolor_t backcolor(art::sys::ansicolor color);

        private:
        egachar* _output;
        u8       _color;
        u8       _ansicolor_lookup[16] = {
            EGA_BLACK, EGA_RED,      EGA_GREEN, EGA_BROWN,  EGA_BLUE,   EGA_MAGENTA,
            EGA_CYAN,  EGA_WHITE,    EGA_GRAY,  EGA_BRED,   EGA_BGREEN, EGA_YELLOW,
            EGA_BBLUE, EGA_BMAGENTA, EGA_BCYAN, EGA_BWHITE,
        };
    };
}