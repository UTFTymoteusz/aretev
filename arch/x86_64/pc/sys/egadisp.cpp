#include "egadisp.hpp"

#include "art/cpu.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

namespace art::arch::sys {
    using namespace art::mm;
    using namespace art::sys;

    egadisp::egadisp() : ttydisp() {
        this->width   = 80;
        this->height  = 25;
        this->_output = (egachar*) kernel_pg.map(
            0x000B8000, this->width * this->height * sizeof(egachar), nullptr, PG_RW);
    }

    egadisp::~egadisp() {
        kernel_pg.unmap(this->_output, this->width * this->height * 2);
    }

    egadisp& egadisp::write(int x, int y, const char* str, ttycolor_t bg, ttycolor_t fg,
                            usz len) {
        egachar* dest  = this->_output + y * this->width + x;
        u8       color = bg | fg;

        len = min<usz>(len, this->width - x);

        for (usz i = 0; i < len; i++) {
            dest[i].c     = str[i];
            dest[i].color = color;
        }

        return *this;
    }

    egadisp& egadisp::clear(char c, ttycolor_t bg, ttycolor_t fg) {
        u8 color = bg | fg;

        for (int i = 0; i < this->width * this->height; i++) {
            this->_output[i].c     = c;
            this->_output[i].color = color;
        }

        return *this;
    }

    egadisp& egadisp::cursor(bool enabled) {
        if (enabled) {
            cpu::outb(0x3D4, 0x0A);
            cpu::outb(0x3D5, (cpu::inb(0x3D5) & 0xC0) | 13);
            cpu::outb(0x3D4, 0x0B);
            cpu::outb(0x3D5, (cpu::inb(0x3D5) & 0xE0) | 14);
        }
        else {
            cpu::outb(0x3D4, 0x0A);
            cpu::outb(0x3D5, 0x20);
        }

        return *this;
    }

    egadisp& egadisp::cursor(int x, int y) {
        u16 pos = x + y * 80;

        cpu::outb(0x3D4, 0x0F);
        cpu::outb(0x3D5, (pos >> 0) & 0xFF);
        cpu::outb(0x3D4, 0x0E);
        cpu::outb(0x3D5, (pos >> 8) & 0xFF);

        return *this;
    }

    ttycolor_t egadisp::forecolor(ansicolor color) {
        return this->_ansicolor_lookup[color] << 0;
    }

    ttycolor_t egadisp::backcolor(ansicolor color) {
        return this->_ansicolor_lookup[color] << 4;
    }
}