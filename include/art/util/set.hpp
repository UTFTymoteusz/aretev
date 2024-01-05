#pragma once

#include "art/int.hpp"
#include "art/mm.hpp"

namespace art {
    /**
     * @brief A collection of values where a value may only occur once.
     * This class does not do any synchronization, please make sure you use a lock or a
     * mutex if need be.
     * @tparam T Contained type.
     */
    template <typename T>
    class set {
        public:
        void add(T val) {
            if (this->has(val))
                return;

            this->_size++;
            this->_buffer = (T*) mm::realloc(this->_buffer, sizeof(T) * _size);
        }

        bool has(T val) {
            for (T* check = this->begin(); check < this->end(); check++) {
                if (val == *check)
                    return true;
            }

            return false;
        }

        bool remove(T val) {
            // TODO
            return false;
        }

        T* begin() {
            return &_buffer[0];
        }

        T* end() {
            return &_buffer[_size];
        }

        private:
        usz _size;
        T*  _buffer;
    };
}