#pragma once

#include "art/int.hpp"
#include "art/mm.hpp"
#include "art/string.hpp"

namespace art {
    /**
     * @brief A circular buffer.
     * This class does not do any synchronization, please make sure you use a lock or a
     * mutex if need be.
     * @tparam T Contained type.
     */
    template <typename T>
    class cbuf {
        public:
        cbuf(usz size) {
            this->_size = size;

            if (size > 0)
                this->_buffer = new T[this->_size];
        }

        ~cbuf() {
            if (this->_buffer)
                delete this->_buffer;
        }

        int read(T* buf, int len, bool noblock = false) {
            int left = len;

            while (left) {
                int avl_len = min<int>(left, (this->_size - this->_wptr));
                avl_len     = min(this->r2w_distance(), avl_len);

                if (avl_len == 0 && noblock)
                    break;

                memcpy(buf, this->_buffer + this->_rptr, avl_len);

                left -= avl_len;
                buf += avl_len;

                this->_rptr += avl_len;

                if (this->_rptr == (int) this->_size)
                    this->_rptr = 0;
            }

            return len - left;
        }

        int write(const T* buf, int len, bool noblock = false) {
            int left = len;

            while (left) {
                int avl_len = min<int>(left, (this->_size - this->_wptr));
                avl_len     = min(this->w2r_distance(), avl_len);

                if (avl_len == 0 && noblock)
                    break;

                memcpy(this->_buffer + this->_wptr, buf, avl_len);

                left -= avl_len;
                buf += avl_len;

                this->_wptr += avl_len;

                if (this->_wptr == (int) this->_size)
                    this->_wptr = 0;
            }

            return len - left;
        }

        void resize(usz size) {
            this->_size   = size;
            this->_buffer = (T*) mm::realloc(this->_buffer, this->_size * sizeof(T));
        }

        private:
        usz _size;
        T*  _buffer;
        int _rptr, _wptr;

        int r2w_distance() {
            if (this->_wptr == this->_rptr)
                return 0;

            if (this->_rptr < this->_wptr)
                return (this->_size + this->_rptr) - this->_wptr;
            else
                return this->_wptr - this->_rptr;
        }

        int w2r_distance() {
            if (this->_wptr == this->_rptr)
                return 0;

            if (this->_rptr < this->_wptr)
                return (this->_size + this->_rptr) - this->_wptr - 1;
            else
                return this->_wptr - this->_rptr - 1;
        }
    };
}