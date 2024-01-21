#pragma once

#include "art/core.hpp"
#include "art/dev/types.hpp"
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

        /**
         * @brief Checks if a devdesc can be accepted by this driver.
         * @return True if the driver can accept this device.
         */
        bool match(shptr<devdesc> devd);

        /**
         * @brief Adds a supported bus to this driver.
         * @param bus Bus name.
         */
        void add_bus(const char* bus);

        /**
         * @brief Adds a supported device id to this driver.
         * This can be either a name, or a specific format address.
         * @param bus Device id.
         */
        void add_devid(const char* id);

        /**
         * @brief Attempts to spawn a new device.
         * @param dev Device descriptor.
         * @return Result error code.
         */
        virtual error_t spawn(shptr<devdesc> devd) = 0;

        protected:
        private:
        vector<const char*> _buses;
        vector<const char*> _ids;
    };
}