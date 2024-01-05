OUTPUT_FORMAT(elf64-littleaarch64)
ENTRY(_start)

MEMORY
{
    flash  (RX)  : ORIGIN = 0x00100000, LENGTH = 0x40000000
    ram    (RWX) : ORIGIN = 0x40000000, LENGTH = 0x40000000
}

SECTIONS
{
    vma = 0xFFFFFFFF80000000; 
    .   = 0x0000000000100000; /* space for dtb */

    _kernelhalfvstart = 0xFFFFFF8000000000;
    _kernelhalfvend   = 0xFFFFFFFFFFFFE000;

    .boot.text ALIGN(4K) : {
        _boot_text_start = .;
        _boot_text_vstart = .;
        *(.boot.text)
        _boot_text_end = .;
        _boot_text_vend = .;
    } >flash

    .boot.rodata ALIGN(4K) : {
        _boot_rodata_start = .;
        _boot_rodata_vstart = .;
        *(.boot.rodata)
        _boot_rodata_end = .;
        _boot_rodata_vend = .;
    } >flash

    . += vma;

    .text ALIGN(4K) : AT(ADDR(.text) - vma) {
        _kernel_text_start = . - vma;
        _kernel_text_vstart = .;
        *(.text*)
        _kernel_text_end = . - vma;
        _kernel_text_vend = .;
    } /* >flash */

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - vma) {
        _kernel_rodata_start = . - vma;
        _kernel_rodata_vstart = .;
        *(.rodata)
        *(.rodata*)
        _kernel_rodata_end = . - vma;
        _kernel_rodata_vend = .;
    } /* >flash */

    /* BEGIN RAM AREA */
    . = 0x0000000040000000;

    .boot.data ALIGN(4K) : {
        _boot_data_start = .;
        _boot_data_vstart = .;
        *(.boot.data)
        _boot_data_end = .;
        _boot_data_vend = .;
    } >ram

    .boot.bss ALIGN(4K) (NOLOAD) : {
        _boot_bss_start = .;
        _boot_bss_vstart = .;
        *(.boot.bss)
        _boot_bss_end = .;
        _boot_bss_vend = .;
    } >ram

    . += vma;

    .data ALIGN(4K) : AT(ADDR(.data) - vma) {
        _kernel_data_start = . - vma;
        _kernel_data_vstart = .;
        *(.data*)
        _kernel_data_end = . - vma;
        _kernel_data_vend = .;
    } /* >ram */

    .bss ALIGN(4K) (NOLOAD) : AT(ADDR(.bss) - vma) {
        _kernel_bss_start = . - vma;
        _kernel_bss_vstart = .;
        *(.COMMON)
        *(.bss*)
        _kernel_bss_end = . - vma;
        _kernel_bss_vend = .;
    } /* >ram */

    .init_array ALIGN(4K) : AT(ADDR(.init_array) - vma) {
        _kernel_init_start = . - vma;
        _kernel_init_vstart = .;
        *(.init_array*)
        _kernel_init_end = . - vma;
        _kernel_init_vend = .;
    } /* >ram */

    /DISCARD/ : {
        *(.eh_frame*);
    }
}