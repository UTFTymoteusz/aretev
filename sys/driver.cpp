#include "art/sys/driver.hpp"

#include "art/data.hpp"
#include "art/sys.hpp"

namespace art::sys {
    driver::driver(const char* name) {
        strlcpy(this->name, name, sizeof(this->name));
    }

    driver::~driver() {}

    bool driver::match(shptr<devdesc> devd) {
        auto bus_match = [&devd](const char* val) { return strcmp(val, devd->bus) == 0; };

        if (find_if_not(this->_buses, bus_match))
            return false;

        auto compatible = devd->attr("compatible", AT_STRING);
        bool matching   = false;

        strtok_iterate(
            compatible.value.stringval, ";", [this, &matching](const char* id) {
                auto ids_match = [id](const char* val) { return strcmp(val, id) == 0; };

                if (find_if(this->_ids, ids_match))
                    matching = true;
            });

        return matching;
    }

    void driver::add_bus(const char* bus) {
        this->_buses.push(bus);
    }

    void driver::add_devid(const char* id) {
        this->_ids.push(id);
    }
}