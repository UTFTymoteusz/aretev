#pragma once

#include "art/dev.hpp"
#include "art/int.hpp"
#include "art/sys.hpp"

namespace art {
    using namespace art::dev;

    class pl011 : public dev::tty {
        public:
        pl011(const char* name, phys_t base);
        ~pl011();

        error_t tty_init();
        usz     tty_read(char* ptr, usz len);
        usz     tty_write(const char* ptr, usz len);
        error_t tty_uninit();

        private:
        const dev::off_t UARTDR    = 0x0000; // Data Register
        const dev::off_t UARTRSR   = 0x0004; // Receive Status Register
        const dev::off_t UARTECR   = 0x0004; // Error Status Register
        const dev::off_t UARTFR    = 0x0018; // Flag Register
        const dev::off_t UARTILPR  = 0x0020; // IrDA Low-Power Counter Register
        const dev::off_t UARTIBRD  = 0x0024; // Integer Baud Rate Register
        const dev::off_t UARTFBRD  = 0x0028; // Fractional Baud Rate Register
        const dev::off_t UARTLCR_H = 0x002C; // Line Control Register
        const dev::off_t UARTCR    = 0x0030; // Control Register
        const dev::off_t UARTIFLS  = 0x0034; // Interrupt FIFO Level Select Register
        const dev::off_t UARTIMSC  = 0x0038; // Interrupt Mask Register
        const dev::off_t UARTRIS   = 0x003C; // Raw Interrupt Status Register
        const dev::off_t UARTMIS   = 0x0040; // Masked Interrupt Status Register
        const dev::off_t UARTICR   = 0x0044; // Interrupt Clear Register
        const dev::off_t UARTDMACR = 0x0048; // DMA Control Registet
        const u32        UARTEN    = 1 << 0;
        const u32        BUSY      = 1 << 3;
        const u32        TXE       = 1 << 8;
        const u32        RXE       = 1 << 9;
        const u32        PEN       = 1 << 1;
        const u32        EPS       = 1 << 2;
        const u32        STP2      = 1 << 3;
        const u32        FEN       = 1 << 4;
        const u32        WLEN      = 1 << 5;
        const u32        SPS       = 1 << 7;

        dev::regs<le<volatile u32>> _io;

        void set_baud(int bauds);
    };

    class pl011drv : public sys::driver {
        public:
        pl011drv();

        error_t spawn(sys::devdesc* dev);
    };
}