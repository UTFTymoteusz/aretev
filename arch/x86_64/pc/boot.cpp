#include "art/data.hpp"

#include "arch/x86_64/none/cpu/gdt.hpp"
#include "boot/loaders/multiboot.h"

namespace art::arch {
    constexpr u32 flags = MULTIBOOT_PAGE_ALIGN;

    extern "C" void _start();

    section(".boot.hdr") aligned(16) multiboot_header header = {
        .magic         = 0x1BADB002,
        .flags         = flags,
        .checksum      = -(0x1BADB002 + flags),
        .header_addr   = 0,
        .load_addr     = 0,
        .load_end_addr = 0,
        .bss_end_addr  = 0,
        .entry_addr    = 0,
        .mode_type     = 0,
        .width         = 0,
        .height        = 0,
        .depth         = 0,
    };
}