#include "tts.hpp"

#include "art/cpu.hpp"

tts16650::tts16650(const char* name, u16 base) : tty(name) {
    this->_base = base;
}

tts16650::~tts16650() {}

error_t tts16650::tty_init() {
    cpu::outb(this->_base + 1, 0x00);
    cpu::outb(this->_base + 3, 0x80);
    cpu::outb(this->_base + 0, 0x03);
    cpu::outb(this->_base + 1, 0x00);
    cpu::outb(this->_base + 3, 0x03);
    cpu::outb(this->_base + 2, 0xC7);
    cpu::outb(this->_base + 4, 0x0F);

    return ENONE;
}

usz tts16650::tty_read(char* ptr, usz len) {
    usz i = 0;

    while (i < len) {
        if (cpu::inb(this->_base + 5) & 0x01)
            ptr[i++] = cpu::inb(this->_base + 0);
    }

    return i;
}

usz tts16650::tty_write(const char* ptr, usz len) {
    usz i = 0;

    while (i < len) {
        if (cpu::inb(this->_base + 5) & 0x20)
            cpu::outb(this->_base + 0, ptr[i++]);
    }

    return i;
}

error_t tts16650::tty_uninit() {
    return ENONE;
}

void tts16650::tty_irq() {}