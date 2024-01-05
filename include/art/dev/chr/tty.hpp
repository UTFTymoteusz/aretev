#pragma once

#include "art/dev.hpp"
#include "art/dev/chrdev.hpp"
#include "art/dev/iovecman.hpp"
#include "art/util.hpp"

#include "api/termios.h"

namespace art::dev {
    /**
     * @brief An abstract class used to implement asynchronous terminal devices.
     */
    class tty : public chrdev {
        public:
        tty(const char* name, int flags = 0);
        virtual ~tty();

        error_t init();
        void    begin(iopkt* packet);
        void    irq();

        protected:
        iopkt*             _rpkt = nullptr;
        off_t              _roff;
        iovecman           _rvecman;
        buffer<char, 1024> _rbuffer;
        iopkt*             _wpkt = nullptr;
        off_t              _woff;
        off_t              _wdbl; // Amount of double characters written out.
        iovecman           _wvecman;
        buffer<char, 128>  _wbuffer;
        termios            _termios;
        int                _column = 0;

        virtual error_t tty_init();
        virtual usz     tty_read(char* ptr, usz len)        = 0;
        virtual usz     tty_write(const char* ptr, usz len) = 0;
        virtual error_t tty_uninit();
        virtual void    tty_irq();
        virtual void    tty_iflag(int flag, bool set);
        virtual void    tty_oflag(int flag, bool set);
        virtual void    tty_cflag(int flag, bool set);
        virtual void    tty_lflag(int flag, bool set);
        virtual speed_t tty_ispeed();
        virtual error_t tty_ispeed(speed_t speed);
        virtual speed_t tty_ospeed();
        virtual error_t tty_ospeed(speed_t speed);

        void iocheck();
        void update(termios& newtermios, bool refresh = false);
    };
}