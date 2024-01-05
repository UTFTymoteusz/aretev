#include "art/boot.hpp"

#include "art/cpu.hpp"
#include "art/data.hpp"

#include "boot.hpp"

namespace art {
    void start(boot_info* info) {
        ksetup(info);
        kmain(0, nullptr);

        cpu::halt();
    }
}