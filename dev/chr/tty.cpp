#include "art/dev/chr/tty.hpp"

#include "art/core.hpp"

namespace art::dev {
    tty::tty(const char* name, int flags) : chrdev(name, flags | DV_MQUEUE | DV_TTY) {
        termios def = {};

        this->_rqueue->max_ongoing(1);
        this->_wqueue->max_ongoing(1);

        notify_ctty(this);

        def.c_iflag = 0;
        def.c_oflag = ONLCR;
        def.c_cflag = 0;
        def.c_lflag = ICANON;

        locking(this->_lock) {
            this->update(def, true);
        }
    }

    tty::~tty() {}

    void tty::begin(iopkt* packet) {
        uninterruptible {
            if (packet->cmd == IO_READ) {
                this->_rvecman.aim(packet->iov, packet->iovcnt, packet->len);
                this->_rbuffer.set(0, 0);
                this->_rpkt = packet;
                this->_roff = 0;
                this->iocheck();
            }
            else if (packet->cmd == IO_WRITE) {
                this->_wvecman.aim(packet->iov, packet->iovcnt, packet->len);
                this->_wbuffer.set(0, 0);
                this->_wpkt = packet;
                this->_woff = 0;
                this->_wdbl = 0;
                this->iocheck();
            }
        }
    }

    error_t tty::init() {
        return this->tty_init();
    }

    void tty::irq() {
        tty::tty_irq();

        locking(this->_lock) this->iocheck();
    }

    error_t tty::tty_init() {
        return ENONE;
    }

    error_t tty::tty_uninit() {
        return ENONE;
    }

    void tty::tty_irq() {}
    void tty::tty_iflag(int, bool) {}
    void tty::tty_oflag(int, bool) {}
    void tty::tty_cflag(int, bool) {}
    void tty::tty_lflag(int, bool) {}

    void tty::iocheck() {
        //

        while (this->_wpkt) {
            if ((this->_termios.c_lflag & ICANON) && !this->_wbuffer.reading()) {
                tcflag_t oflag = this->_termios.c_oflag;

                while (this->_wbuffer.space() >= 2) {
                    char c;
                    usz  len = this->_wvecman.read(&c, sizeof(c));
                    if (len == 0)
                        break;

                    if (oflag & ONLCR && c == '\n') {
                        this->_wbuffer.write("\r", sizeof(c));
                        this->_wdbl++;
                    }

                    if (oflag & OCRNL && c == '\r')
                        c = '\n';

                    if (oflag & ONOCR && c == '\r' && this->_column == 0) {
                        this->_woff++;
                        continue;
                    }

                    if (oflag & ONLRET && c == '\n') {
                        c = '\r';
                        continue;
                    }

                    this->_wbuffer.write(&c, sizeof(c));
                }

                this->_wbuffer.ready();
            }
            else if (!(this->_termios.c_lflag & ICANON) && !this->_wbuffer.reading()) {
                while (this->_wbuffer.space()) {
                    usz len = this->_wvecman.read(this->_wbuffer.pointer(),
                                                  this->_wbuffer.space());
                    if (len == 0)
                        break;

                    this->_wbuffer.write(len);
                }

                this->_wbuffer.ready();
            }

            usz len = this->tty_write(this->_wbuffer.pointer(), this->_wbuffer.left());
            if (len == 0)
                break;

            this->_wbuffer.read(len);
            this->_woff += len;

            if (this->_woff - this->_wdbl == this->_wpkt->len) {
                this->complete(this->_wpkt, this->_woff - this->_wdbl);

                this->_wpkt = nullptr;
                this->_woff = 0;
                this->_wdbl = 0;
            }
        }
    }

    void tty::update(termios& newtermios, bool refresh) {
        bitman<tcflag_t> old_oflags(this->_termios.c_oflag);
        bitman<tcflag_t> new_oflags(newtermios.c_oflag);
        bitman<tcflag_t> old_lflags(this->_termios.c_lflag);
        bitman<tcflag_t> new_lflags(newtermios.c_lflag);

        for (int i = 0; i < 32; i++) {
            if (old_oflags.get(i) == new_oflags.get(i) && !refresh)
                continue;

            switch (1 << i) {
            case ONLCR:
                this->tty_lflag(1 << i, new_oflags.get(i));
                break;
            default:
                break;
            }

            old_oflags.set(i, new_oflags.get(i));
        }

        for (int i = 0; i < 32; i++) {
            if (old_lflags.get(i) == new_lflags.get(i) && !refresh)
                continue;

            switch (1 << i) {
            case ICANON:
                this->tty_lflag(1 << i, new_lflags.get(i));
                break;
            default:
                break;
            }

            old_lflags.set(i, new_lflags.get(i));
        }
    }

    speed_t tty::tty_ispeed() {
        return B0;
    }

    error_t tty::tty_ispeed(speed_t speed) {
        return ENOSYS;
    }

    speed_t tty::tty_ospeed() {
        return B0;
    }

    error_t tty::tty_ospeed(speed_t speed) {
        return ENOSYS;
    }
}