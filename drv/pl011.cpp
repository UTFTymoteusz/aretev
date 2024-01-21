#include "drv/pl011.hpp"

#include "art/mm.hpp"

namespace art {
    pl011::pl011(const char* name, phys_t base) : dev::tty(name) {
        this->_io = dev::regs<le<volatile u32>>(base, 0x1000);

        this->_io[UARTCR] = ~UARTEN;

        while (this->_io[UARTFR] & BUSY)
            proc::yield();

        this->_io[UARTLCR_H] &= ~FEN;

        this->set_baud(115200);

        this->_io[UARTLCR_H] |= (0b11 << 5) | STP2;
        this->_io[UARTIMSC]  = 0x7FF;
        this->_io[UARTDMACR] = 0x00;
        this->_io[UARTCR]    = TXE | UARTEN;

        this->_io[UARTDR] = 0xC4;
        this->_io[UARTDR] = 0x99;
        this->_io[UARTDR] = 0xC5;
        this->_io[UARTDR] = 0x9B;
        this->_io[UARTDR] = 0xC4;
        this->_io[UARTDR] = 0x85;
        this->_io[UARTDR] = 0xC4;
        this->_io[UARTDR] = 0x87;
        this->_io[UARTDR] = 0xC5;
        this->_io[UARTDR] = 0xBC;
        this->_io[UARTDR] = 0x0A;
        this->_io[UARTDR] = 0x0D;
    }

    pl011::~pl011() {}

    error_t pl011::tty_init() {
        // this->_io_regs.read();

        return ENONE;
    }

    usz pl011::tty_read(char* ptr, usz len) {
        return 0;
    }

    usz pl011::tty_write(const char* ptr, usz len) {
        for (usz i = 0; i < len; i++)
            this->_io[UARTDR] = ptr[i];

        return len;
    }

    error_t pl011::tty_uninit() {
        return ENONE;
    }

    void pl011::set_baud(int bauds) {
        u64 reference = 24000000; // hz
        u32 value     = (reference << 6) / (16 * bauds);
        int frac      = value & 0x3F;
        int inte      = (value >> 6) & 0xFFFF;

        this->_io[UARTFBRD] = frac;
        this->_io[UARTIBRD] = inte;
    }

    pl011drv::pl011drv() : sys::driver("pl011") {}

    error_t pl011drv::spawn(shptr<sys::devdesc> devd) {
        auto memory = devd->res(0);
        if (!memory.has_value)
            return EINVAL;

        auto dev = new pl011("tts0", memory.value.start);
        dev::register_device(dev);
        // devd->representation = dev;

        return ENONE;
    }
}