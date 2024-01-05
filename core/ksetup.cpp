#include "art/boot.hpp"

#include "dev/dev.hpp"
#include "mm/mm.hpp"
#include "sys/sys.hpp"

namespace art {
    void ksetup(boot_info* info) {
        mm::init(info);
        sys::init(info);
        dev::init(info);
    }
}