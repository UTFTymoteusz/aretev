#pragma once

#include "art/sys/types.hpp"

namespace art::sys {
    /**
     * @brief A class responsible for creating devices.
     */
    class driver {
        public:
        char name[16];
        int  flags;

        driver(const char* name);
        virtual ~driver();

        protected:
        private:
    };
}