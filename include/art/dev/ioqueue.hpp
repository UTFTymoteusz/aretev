#pragma once

#include "art/dev/types.hpp"
#include "art/proc/lock.hpp"
#include "art/util/cbuf.hpp"

namespace art::dev {
    class ioqueue {
        public:
        ioqueue();
        ~ioqueue();

        bool   push(iopkt* packet);
        iopkt* next();
        void   completed(iopkt* packet);
        void   max_ongoing(int max);
        int    space();

        private:
        proc::lock    _lock;
        int           _ongoing_cur;
        int           _ongoing_max;
        llist<iopkt*> _queue;
    };
}