#pragma once

namespace art {
    template <typename T>
    class shptr {
        public:
        shptr() {
            _val = nullptr;
        }

        T& operator*() {
            return *_val;
        }

        T* operator->() {
            return _val;
        }

        private:
        T* _val;
    };
}