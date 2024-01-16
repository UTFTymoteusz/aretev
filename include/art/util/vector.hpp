#pragma once

#include "art/dbg.hpp"
#include "art/mm.hpp"

namespace art {
    /**
     * @brief An equivalent to C++'s std::vector.
     * This class does not do any synchronization, please make sure you use a lock or a
     * mutex if need be.
     * @tparam T Contained type.
     */
    template <typename T>
    class vector {
        public:
        /**
         * @brief Pushes an element to the back of the vector.
         * @param val Element to push.
         */
        void push(T& val) {
            this->_count++;
            this->_array = (T*) mm::realloc(this->_array, this->_count * sizeof(T));
            this->_array[this->_count - 1] = val;
        }

        /**
         * @brief Returns the numbers of elements.
         * @return Count of elements.
         */
        usz size() {
            return this->_count;
        }

        T* get() {
            return this->_array;
        }

        T* begin() {
            return &this->_array[0];
        }

        T* end() {
            return &this->_array[this->_count];
        }

        private:
        T*  _array = nullptr;
        usz _count = 0;
    };
}