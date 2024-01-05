#pragma once

#include "art/proc/types.hpp"

template <typename T>
class _lockbong {
    public:
    _lockbong(T& val) : ref(val) {
        ref.acquire();
    }

    ~_lockbong() {
        ref.release();
    }

    private:
    T& ref;
};

#define _concat(a, b) a##b
#define concat(a, b) _concat(a, b)

#define locking(lock)        \
    for (int __l = ({        \
             lock.acquire(); \
             0;              \
         });                 \
         __l < 1; ({         \
             lock.release(); \
             __l++;          \
         }))

#define guard(lock, T) auto concat(__guard, __LINE__) = _lockbong<T>(lock)

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