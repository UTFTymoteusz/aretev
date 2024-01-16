#pragma once

#include "art/cpu.hpp"

namespace art::proc {
    template <typename T>
    class lock_guard {
        public:
        lock_guard(T& val) : ref(val) {
            ref.acquire();
        }

        ~lock_guard() {
            ref.release();
        }

        private:
        T& ref;
    };

    class int_guard {
        public:
        int_guard(bool interrupts) {
            had_interrupts = cpu::check_interrupts();
            cpu::interrupts(interrupts);
        }

        ~int_guard() {
            cpu::interrupts(had_interrupts);
        }

        private:
        bool had_interrupts;
    };
}