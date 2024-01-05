#include "art/core/module.hpp"

namespace art {
    proc::lock       module_lock;
    vector<kmodule*> modules;

    optional<int> load_module(void* image) {
        return ENOEXEC;
    }

    error_t unload_module(int handle) {
        return ENONE;
    }
}