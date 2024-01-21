#include "art/mm.hpp"

#include "art/boot.hpp"
#include "art/cpu.hpp"
#include "art/data.hpp"
#include "art/string.hpp"

#include "mm/heap/heap.hpp"
#include "mm/phys/phys.hpp"

namespace art::mm {
    pagetbl kernel_pg;

    /**
     * @brief Initializes memory management.
     */
    section(".boot.text") void init(boot_info* info) {
        phys::init(info);
        kernel_pg.boot_fill();
        heap::init();
        phys::migrate(info);
    }
}