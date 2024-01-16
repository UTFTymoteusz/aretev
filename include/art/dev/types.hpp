#pragma once

#include "art/errno.hpp"
#include "art/int.hpp"
#include "art/proc.hpp"
#include "art/util.hpp"

namespace art::dev {
    typedef usz blkssz_t;
    typedef usz blkcnt_t;
    typedef usz off_t;

    enum iocmd {
        IO_READ,
        IO_WRITE,
        IO_INIT,
        IO_UNINI,
        IO_CTL,
        IO_WAIT,
    };

    struct iovec {
        void* base;
        usz   len;
    };

    struct iopkt {
        iovec* iov;
        int    iovcnt;
        usz    len;
        iocmd  cmd;
        off_t  offset;
        void*  data;
        void*  context;
        void (*cb)(iopkt*); // Callback function. Will always be called with interrupts
                            // turned off.
        error_t       error;
        usz           result;
        volatile bool completed;
        bool          noblock;
        iopkt*        next;

        void complete(usz result) {
            this->error  = ENONE;
            this->result = result;

            if (this->cb) {
                proc::int_guard ig(false);
                this->cb(this);
            }

            this->completed = true;
        }

        void complete(error_t error) {
            this->error  = error;
            this->result = 0;

            if (this->cb) {
                proc::int_guard ig(false);
                this->cb(this);
            }

            this->completed = true;
        }
    };
}