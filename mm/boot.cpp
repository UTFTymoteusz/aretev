#include "boot.hpp"

#include "art/data.hpp"
#include "art/mm.hpp"

namespace art::mm {
    // *sigh*
    extern "C" char _kernelhalfvstart, _kernelhalfvend;
    extern "C" char _boot_text_start;
    extern "C" char _boot_text_vstart;
    extern "C" char _boot_text_end;
    extern "C" char _boot_text_vend;
    extern "C" char _boot_rodata_start;
    extern "C" char _boot_rodata_vstart;
    extern "C" char _boot_rodata_end;
    extern "C" char _boot_rodata_vend;
    extern "C" char _boot_data_start;
    extern "C" char _boot_data_vstart;
    extern "C" char _boot_data_end;
    extern "C" char _boot_data_vend;
    extern "C" char _boot_bss_start;
    extern "C" char _boot_bss_vstart;
    extern "C" char _boot_bss_end;
    extern "C" char _boot_bss_vend;
    extern "C" char _kernel_text_start;
    extern "C" char _kernel_text_vstart;
    extern "C" char _kernel_text_end;
    extern "C" char _kernel_text_vend;
    extern "C" char _kernel_rodata_start;
    extern "C" char _kernel_rodata_vstart;
    extern "C" char _kernel_rodata_end;
    extern "C" char _kernel_rodata_vend;
    extern "C" char _kernel_data_start;
    extern "C" char _kernel_data_vstart;
    extern "C" char _kernel_data_end;
    extern "C" char _kernel_data_vend;
    extern "C" char _kernel_bss_start;
    extern "C" char _kernel_bss_vstart;
    extern "C" char _kernel_bss_end;
    extern "C" char _kernel_bss_vend;
    extern "C" char _kernel_init_start;
    extern "C" char _kernel_init_vstart;
    extern "C" char _kernel_init_end;
    extern "C" char _kernel_init_vend;

    section(".boot.data") addr_t b_kernelhalf_start     = (addr_t) &_kernelhalfvstart;
    section(".boot.data") addr_t b_kernelhalf_end       = (addr_t) &_kernelhalfvend;
    section(".boot.data") phys_t b_boot_text_start      = (phys_t) &_boot_text_start;
    section(".boot.data") addr_t b_boot_text_vstart     = (addr_t) &_boot_text_vstart;
    section(".boot.data") phys_t b_boot_text_end        = (phys_t) &_boot_text_end;
    section(".boot.data") addr_t b_boot_text_vend       = (addr_t) &_boot_text_vend;
    section(".boot.data") phys_t b_boot_rodata_start    = (phys_t) &_boot_rodata_start;
    section(".boot.data") addr_t b_boot_rodata_vstart   = (addr_t) &_boot_rodata_vstart;
    section(".boot.data") phys_t b_boot_rodata_end      = (phys_t) &_boot_rodata_end;
    section(".boot.data") addr_t b_boot_rodata_vend     = (addr_t) &_boot_rodata_vend;
    section(".boot.data") phys_t b_boot_data_start      = (phys_t) &_boot_data_start;
    section(".boot.data") addr_t b_boot_data_vstart     = (addr_t) &_boot_data_vstart;
    section(".boot.data") phys_t b_boot_data_end        = (phys_t) &_boot_data_end;
    section(".boot.data") addr_t b_boot_data_vend       = (addr_t) &_boot_data_vend;
    section(".boot.data") phys_t b_boot_bss_start       = (phys_t) &_boot_bss_start;
    section(".boot.data") addr_t b_boot_bss_vstart      = (addr_t) &_boot_bss_vstart;
    section(".boot.data") phys_t b_boot_bss_end         = (phys_t) &_boot_bss_end;
    section(".boot.data") addr_t b_boot_bss_vend        = (addr_t) &_boot_bss_vend;
    section(".boot.data") phys_t b_kernel_text_start    = (phys_t) &_kernel_text_start;
    section(".boot.data") addr_t b_kernel_text_vstart   = (addr_t) &_kernel_text_vstart;
    section(".boot.data") phys_t b_kernel_text_end      = (phys_t) &_kernel_text_end;
    section(".boot.data") addr_t b_kernel_text_vend     = (addr_t) &_kernel_text_vend;
    section(".boot.data") phys_t b_kernel_rodata_start  = (phys_t) &_kernel_rodata_start;
    section(".boot.data") addr_t b_kernel_rodata_vstart = (addr_t) &_kernel_rodata_vstart;
    section(".boot.data") phys_t b_kernel_rodata_end    = (phys_t) &_kernel_rodata_end;
    section(".boot.data") addr_t b_kernel_rodata_vend   = (addr_t) &_kernel_rodata_vend;
    section(".boot.data") phys_t b_kernel_data_start    = (phys_t) &_kernel_data_start;
    section(".boot.data") addr_t b_kernel_data_vstart   = (addr_t) &_kernel_data_vstart;
    section(".boot.data") phys_t b_kernel_data_end      = (phys_t) &_kernel_data_end;
    section(".boot.data") addr_t b_kernel_data_vend     = (addr_t) &_kernel_data_vend;
    section(".boot.data") phys_t b_kernel_bss_start     = (phys_t) &_kernel_bss_start;
    section(".boot.data") addr_t b_kernel_bss_vstart    = (addr_t) &_kernel_bss_vstart;
    section(".boot.data") phys_t b_kernel_bss_end       = (phys_t) &_kernel_bss_end;
    section(".boot.data") addr_t b_kernel_bss_vend      = (addr_t) &_kernel_bss_vend;
    section(".boot.data") phys_t b_kernel_init_start    = (phys_t) &_kernel_init_start;
    section(".boot.data") addr_t b_kernel_init_vstart   = (addr_t) &_kernel_init_vstart;
    section(".boot.data") phys_t b_kernel_init_end      = (phys_t) &_kernel_init_end;
    section(".boot.data") addr_t b_kernel_init_vend     = (addr_t) &_kernel_init_vend;
}