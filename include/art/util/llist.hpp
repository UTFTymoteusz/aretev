#pragma once

namespace art {
    /**
     * @brief A linked list.
     * This class does not do any synchronization, please make sure you use a lock or a
     * mutex if need be.
     * @tparam T Contained type.
     */
    template <typename T>
    class llist {
        public:
        /**
         * @brief Removes an item from the front of the list.
         * @return Removed item or T() if the list was empty.
         */
        T pop_front() {
            T val = this->_first;

            if (!val)
                return T();

            this->_first = this->_first->next;
            this->_size--;

            return val;
        }

        /**
         * @brief Adds an item to the back of the list.
         * @param val Item to add.
         */
        void push_back(T val) {
            this->_size++;

            if (!this->_first) {
                this->_first = val;
                this->_last  = val;

                val->next = nullptr;
                return;
            }

            this->_last->next = val;
            this->_last       = val;
        }

        private:
        T   _first;
        T   _last;
        usz _size;
    };
}