#pragma once

#include "art/int.hpp"

namespace art {
    class shcount {
        public:
        constexpr shcount(int refs) {
            this->_refs = refs;
        }

        void inc() {
            atomic_add(&_refs, 1);
        }

        bool dec() {
            return atomic_sub_fetch(&_refs, 1) == 0;
        }

        int ref() {
            return atomic_read(&_refs);
        }

        private:
        int _refs;
    };

    template <typename T>
    class shptr {
        public:
        constexpr shptr() {
            this->_val     = nullptr;
            this->_counter = nullptr;
        }

        constexpr shptr(T* ptr) {
            this->_val     = ptr;
            this->_counter = new shcount(1);
        }

        constexpr shptr(T* ptr, shcount* counter) {
            this->_val     = ptr;
            this->_counter = counter;

            if (this->_counter)
                this->_counter->inc();
        }

        constexpr shptr(const shptr& sp) {
            this->_val     = sp._val;
            this->_counter = sp._counter;

            if (this->_counter)
                this->_counter->inc();
        }

        constexpr ~shptr() {
            if (!this->_counter)
                return;

            if (this->_counter->dec())
                cleanup();
        }

        T& operator*() {
            return *_val;
        }

        T* operator->() {
            return _val;
        }

        template <typename U>
        constexpr operator shptr<U>() {
            return shptr<U>((U*) this->_val, this->_counter);
        }

        constexpr operator bool() {
            return _val != nullptr;
        }

        constexpr shptr<T>& operator=(const shptr<T>& sp) {
            this->_val     = (T*) sp._val;
            this->_counter = sp._counter;

            if (this->_counter)
                this->_counter->inc();

            return *this;
        }

        private:
        T*       _val;
        shcount* _counter;

        void cleanup() {
            if (this->_val)
                delete this->_val;

            if (this->_counter)
                delete this->_counter;

            this->_val     = nullptr;
            this->_counter = nullptr;
        }
    };
}