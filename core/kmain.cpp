#include "art/core.hpp"
#include "art/cpu.hpp"
#include "art/dev.hpp"
#include "art/sys.hpp"

#include "afx/elf.h"

namespace art {
    void* elf_alloccb(void* data, void* vaddr, usz size, int align, u32 flags) {
        printk("aaa %p, sz %u, flags %x\n", vaddr, size, flags);
        return nullptr;
    }

    bool elf_check(void* data, const char* name) {
        printk("aaa %s\n", name);
        return true;
    }

    void image_test(phys_t paddr) {
        void*     mapped = mm::kernel_pg.map(paddr, 1024 * 512, nullptr);
        afx::elf* elf    = afx::elf_parse(mapped, 1024 * 512);
        u8*       bytes  = (u8*) mapped;

        printk("%s\n", afx::elf_check(elf, nullptr) ? "ELF success" : "ELF failure");
        printk("%c%c%c%c\n", bytes[0], bytes[1], bytes[2], bytes[3]);

        if (elf) {
            printk("entry: %p\n", elf->entry);
            printk("image: %p\n", elf->image);

            afx::elf_link(elf, nullptr, elf_check, nullptr, nullptr);
            // afx::elf_load(elf, nullptr, elf_alloccb, nullptr);
        }
    }

    int kmain(int, char*[]) {
        printk("Entered %s()\n", __func__);
        // image_test(0x800000);

        cpu::halt();

        return 0;
    }
}
