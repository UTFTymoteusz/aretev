#pragma once

#include "art/dev/types.hpp"

namespace art::dev {
    class iovecman {
        public:
        iovecman();
        iovecman(iovec* iov, int iovcnt, usz len);

        usz  read(void* buffer, usz len);
        usz  write(const void* buffer, usz len);
        void aim(iovec* iov, int iovcnt, usz len);

        private:
        iovec* _iov;       // iovec array.
        int    _iovcnt;    // iovec count.
        usz    _remaining; // Bytes remaining.
        int    _index;     // Index in _iov.
        usz    _offset;    // Offset in the current iovec.
    };
}