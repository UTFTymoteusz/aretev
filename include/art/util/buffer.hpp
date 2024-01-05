#pragma once

#include "art/int.hpp"
#include "art/string.hpp"

namespace art {
    /**
     * @brief A basic stateful buffer.
     * This buffer alternates between reading and writing states. Standard atomicity
     * warnings apply here too, use a lock or a mutex if need be.
     * @tparam T Contained type.
     * @tparam L Size of the buffer.
     */
    template <typename T, usz L>
    class buffer {
        public:
        T buffer[L];

        void set(usz offset, usz len) {
            this->_offset = offset;
            this->_len    = len;
        }

        /**
         * @brief Marks the buffer as ready to be read from.
         */
        void ready() {
            this->_reading = true;
        }

        /**
         * @brief Marks the buffer as ready to be written to again and resets the offset
         * and the length.
         */
        void exhausted() {
            this->_reading = false;
            this->_offset  = 0;
            this->_len     = 0;
        }

        /**
         * @brief Checks whether the buffer is in the reading state.
         */
        bool reading() {
            return this->_reading;
        }

        /**
         * @brief Checks the amount of characters left in the buffer.
         */
        usz left() {
            return this->_len - this->_offset;
        }

        /**
         * @brief Advances the read pointer.
         * This method will automatically put the buffer into the writing state when the
         * buffer has been fully exhausted.
         */
        void read(usz len) {
            this->_offset += len;

            if (this->left() == 0)
                this->exhausted();
        }

        void write(usz len) {
            this->_len += len;
        }

        usz write(const char* str, usz len) {
            // TODO: assert(!this->_reading);

            len = min(len, L - this->_len);

            memcpy(this->buffer + this->_len, str, len);
            this->_len += len;

            return len;
        }

        /**
         * @brief Checks the amount of space left for writing.
         */
        usz space() {
            return L - this->_len;
        }

        T* pointer() {
            return this->buffer + this->_offset;
        }

        bool empty() {
            return this->_offset == this->_len;
        }

        usz capacity() {
            return L;
        }

        private:
        usz  _offset  = 0;
        usz  _len     = 0;
        bool _reading = false;
    };
}