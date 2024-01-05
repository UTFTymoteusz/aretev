#pragma once

#include "art/int.hpp"
#include "art/mm/types.hpp"

namespace art::mm {
    extern phys_t boot_text_start;
    extern addr_t boot_text_vstart;
    extern phys_t boot_text_end;
    extern addr_t boot_text_vend;
    extern phys_t boot_rodata_start;
    extern addr_t boot_rodata_vstart;
    extern phys_t boot_rodata_end;
    extern addr_t boot_rodata_vend;
    extern phys_t boot_data_start;
    extern addr_t boot_data_vstart;
    extern phys_t boot_data_end;
    extern addr_t boot_data_vend;
    extern phys_t boot_bss_start;
    extern addr_t boot_bss_vstart;
    extern phys_t boot_bss_end;
    extern addr_t boot_bss_vend;
    extern phys_t kernel_text_start;
    extern addr_t kernel_text_vstart;
    extern phys_t kernel_text_end;
    extern addr_t kernel_text_vend;
    extern phys_t kernel_rodata_start;
    extern addr_t kernel_rodata_vstart;
    extern phys_t kernel_rodata_end;
    extern addr_t kernel_rodata_vend;
    extern phys_t kernel_data_start;
    extern addr_t kernel_data_vstart;
    extern phys_t kernel_data_end;
    extern addr_t kernel_data_vend;
    extern phys_t kernel_bss_start;
    extern addr_t kernel_bss_vstart;
    extern phys_t kernel_bss_end;
    extern addr_t kernel_bss_vend;
    extern phys_t kernel_init_start;
    extern addr_t kernel_init_vstart;
    extern phys_t kernel_init_end;
    extern addr_t kernel_init_vend;
}