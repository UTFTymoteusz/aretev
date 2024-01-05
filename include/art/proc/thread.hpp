#pragma once

#include "art/proc/lock.hpp"
#include "art/proc/types.hpp"

namespace art::proc {
    class thread {
        public:
        private:
        lock                 _lock;
        state                _state;
        arch::proc::context* _context;

        friend void reschedule();
    };
}
