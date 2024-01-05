#include "dev/chr/tty/vtty.hpp"

namespace art::dev {
    vtty::vtty(const char* name, sys::ttydisp* display) : tty(name, 0) {
        this->_display    = display;
        this->_backbuffer = new vchar[display->height * display->width];
        this->_fg         = this->_display->forecolor(sys::ANSI_BWHITE);
        this->_bg         = this->_display->forecolor(sys::ANSI_BLACK);

        for (int y = 0; y < display->height; y++) {
            this->clear(y);
            this->commit(0, y, display->width);
        }
    }

    vtty::~vtty() {
        if (this->_backbuffer)
            delete this->_backbuffer;
    }

    void vtty::commit(int x, int y, int len) {
        int    start = x;
        int    end   = min(x + len, this->_display->width);
        vchar* line  = this->_backbuffer + (this->_display->width * y);

        for (int i = start; i < end; i++)
            this->_display->write(i, y, &line[i].chr, line[i].bg, line[i].fg, 1);
    }

    void vtty::scroll(int amount) {
        int w = this->_display->width;
        int h = this->_display->height;

        if (amount >= h || -amount >= h) {
            for (int y = 0; y < h; y++) {
                this->clear(y);
                this->commit(0, y, w);
            }

            return;
        }

        if (amount == 0)
            return;

        if (amount > 0) {
            memcpy(this->_backbuffer, this->_backbuffer + amount * w,
                   sizeof(vchar) * w * (h - amount));

            for (int y = h - amount; y < h; y++)
                this->clear(y);

            for (int y = 0; y < h; y++)
                this->commit(0, y, w);
        }
    }

    void vtty::clear(int y) {
        vchar* line = this->_backbuffer + (this->_display->width * y);

        for (int x = 0; x < this->_display->width; x++) {
            line[x].chr = '\0';
            line[x].fg  = this->_fg;
            line[x].bg  = this->_bg;
        }
    }

    void vtty::putchar(int x, int y, char c) {
        vchar& vc = this->_backbuffer[x + y * this->_display->width];

        vc.chr = c;
        vc.bg  = this->_bg;
        vc.fg  = this->_fg;
    }

    usz vtty::tty_read(char* ptr, usz len) {
        return 0;
    }

    usz vtty::tty_write(const char* ptr, usz len) {
        int startx = this->_x;
        int starty = this->_y;

        for (usz i = 0; i < len; i++) {
            switch (ptr[i]) {
            case '\n':
                this->commit(startx, starty, this->_x - startx);
                this->_y++;
                starty = this->_y;
                break;
            case '\r':
                this->commit(startx, starty, this->_x - startx);
                this->_x = 0;
                startx   = this->_x;
                break;
            default:
                this->putchar(this->_x, this->_y, ptr[i]);
                this->_x++;
                break;
            }

            if (this->_x >= this->_display->width) {
                this->commit(startx, starty, this->_x - startx);
                this->_x = 0;
                this->_y++;
                startx = this->_x;
                starty = this->_y;
            }

            if (this->_y >= this->_display->height) {
                scroll(1);
                this->_y--;
                startx = this->_x;
                starty = this->_y;
            }
        }

        this->commit(startx, starty, this->_x - startx);
        this->_display->cursor(this->_x, this->_y);

        return len;
    }
}