#pragma once

#include "art/data.hpp"
#include "art/proc/lock.hpp"
#include "art/sys/types.hpp"
#include "art/util.hpp"

namespace art::sys {
    /**
     * @brief A class that represents a physical device.
     * This class is intended to only describe a device.
     */
    class devdesc {
        public:
        char               name[32];
        const char*        bus;
        int                flags;
        vector<attribute*> attributes;
        vector<resource>   resources;
        devdesc*           parent = nullptr;
        driver*            owner  = nullptr;

        devdesc(const char* name, int flags = 0);
        virtual ~devdesc();

        attribute*          push(attribute* attr);
        void                push(resource&& attr);
        optional<attribute> attr(const char* name);
        optional<resource>  res(int id);

        protected:
        private:
        proc::lock _lock;
        int        _attrcnt;
    };
}