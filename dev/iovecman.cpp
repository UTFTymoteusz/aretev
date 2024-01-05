#include "art/dev/iovecman.hpp"

namespace art::dev {
    iovecman::iovecman() {}

    iovecman::iovecman(iovec* iov, int iovcnt, usz len) {
        this->_iov       = iov;
        this->_iovcnt    = iovcnt;
        this->_remaining = len;
        this->_index     = 0;
        this->_offset    = 0;
    }

    usz iovecman::read(void* buffer, usz len) {
        usz   clamped = min(len, this->_remaining);
        usz   left    = clamped;
        char* dst     = (char*) buffer;

        while (left > 0) {
            if (this->_index == this->_iovcnt)
                break;

            usz avl = min(left, this->_iov[this->_index].len - this->_offset);
            if (avl == 0)
                break;

            memcpy(dst, (char*) this->_iov[this->_index].base + this->_offset, avl);

            dst += avl;
            left -= avl;

            this->_offset += avl;
            this->_remaining -= avl;

            if (this->_offset == this->_iov[this->_index].len) {
                this->_index++;
                this->_offset = 0;
            }
        }

        return clamped - left;
    }

    usz iovecman::write(const void* buffer, usz len) {
        usz         clamped = min(len, this->_remaining);
        usz         left    = clamped;
        const char* src     = (const char*) buffer;

        while (left > 0) {
            if (this->_index == this->_iovcnt)
                break;

            usz avl = min(left, this->_iov[this->_index].len - this->_offset);
            if (avl == 0)
                break;

            memcpy((char*) this->_iov[this->_index].base + this->_offset, src, avl);

            src += avl;
            left -= avl;

            this->_offset += avl;
            this->_remaining -= avl;

            if (this->_offset == this->_iov[this->_index].len) {
                this->_index++;
                this->_offset = 0;
            }
        }

        return clamped - left;
    }

    void iovecman::aim(iovec* iov, int iovcnt, usz len) {
        this->_iov       = iov;
        this->_iovcnt    = iovcnt;
        this->_remaining = len;
        this->_index     = 0;
        this->_offset    = 0;
    }
}