#pragma once

#include "art/data.hpp"
#include "art/errno.hpp"
#include "art/proc.hpp"
#include "art/util/vector.hpp"

namespace art {
    struct modsecti {
        void* start;
        usz   len;
    };

    class kmodule {
        const char* name;
        int         (*init)(int argc, const char* argv[]);
        void        (*exit)();
        modsecti*   sections;
    };

    extern proc::lock       module_lock;
    extern vector<kmodule*> modules;

    optional<int> load_module(void* image);
    error_t       unload_module(int handle);
}