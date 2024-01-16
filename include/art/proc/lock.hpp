#pragma once

#include "art/proc/types.hpp"

namespace art::proc {
    /**
     * @brief A lock that prevents context switching when acquired.
     */
    class lock {
        public:
        void acquire();
        bool try_acquire();
        void release();

        private:
        proc::thread* _holder = nullptr;
        int           _lock   = 0;
    };
}