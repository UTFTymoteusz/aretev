#pragma once

#include "art/data.hpp"
#include "art/proc/lock.hpp"
#include "art/sys/types.hpp"

namespace art::sys {
    /**
     * @brief A class that represents a physical device.
     * This class is intended to only describe a device.
     */
    class device {
        public:
        char        name[16];
        int         flags;
        attribute** attributes;

        device(const char* name, int flags = 0);
        virtual ~device();

        optional<attribute> attr(const char* name);

        protected:
        attribute* push(attribute* attr);

        private:
        proc::lock _lock;
        int        _attrcnt;
    };
}