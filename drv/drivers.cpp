#include "drv/pl011.hpp"

#define driver(name, bus, id)            \
    {                                    \
        auto driver = mm::share<name>(); \
        driver->add_bus(bus);            \
        driver->add_devid(id);           \
        sys::register_driver(driver);    \
    }

namespace art {
    void register_drivers() {
        driver(pl011drv, "main", "arm,pl011");
    }
}