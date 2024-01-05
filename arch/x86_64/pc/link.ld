OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

MEMORY
{
    ivt     (RW) : ORIGIN = 0x00000000, LENGTH = 1024
    bda     (R)  : ORIGIN = 0x00000400, LENGTH = 256
    mem0    (RWX): ORIGIN = 0x00000500, LENGTH = 30463
    bootsec (RWX): ORIGIN = 0x00007C00, LENGTH = 512
    mem1    (RWX): ORIGIN = 0x00007E00, LENGTH = 492031
    ebda    (RW) : ORIGIN = 0x00080000, LENGTH = 131072
    vram    (RW) : ORIGIN = 0x000A0000, LENGTH = 131072
    vbios   (R)  : ORIGIN = 0x000C0000, LENGTH = 32768
    biosext (R)  : ORIGIN = 0x000C8000, LENGTH = 163840
    bios    (R)  : ORIGIN = 0x000F0000, LENGTH = 65536
    mem2    (RWX): ORIGIN = 0x00100000, LENGTH = 14680064
    isahole (RW) : ORIGIN = 0x00F00000, LENGTH = 1048576
    mem3    (RWX): ORIGIN = 0x01000000, LENGTH = 15728640
}

SECTIONS
{
    vma = 0xFFFFFFFF80000000; 
    .   = 0; # ld????
    .   = 0x0000000000007E00;

    _kernelhalfvstart = 0xFFFF800000000000;
    _kernelhalfvend   = 0xFFFFFFFFFFFFE000;

    .boot.hdr ALIGN(4) : {
        _bootstart = .;
        _bootvstart = .;
        *(.boot.hdr)
    } >mem1

    .boot.text ALIGN(4K) : {
        *(.boot.text)
    } >mem1

    .boot.rodata ALIGN(4K) : {
        *(.boot.rodata)
    } >mem1

    .boot.data ALIGN(4K) : {
        *(.boot.data)
    } >mem1

    .boot.bss ALIGN(4K) (NOLOAD) : {
        *(.boot.bss)
        _bootend = .;
        _bootvend = .;
    } >mem1

    .  = 0x0000000000100000;
    . += vma;

    .text ALIGN(4K) : AT(ADDR(.text) - vma) {
        _kernelstart = . - vma;
        _kernelvstart = .;
        *(.text)
        *(.text*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - vma) {
        *(.rodata)
        *(.rodata*)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - vma) {
        *(.data)
        *(.data*)
    }

    .bss ALIGN(4K) (NOLOAD) : AT(ADDR(.bss) - vma) {
        *(.COMMON)
        *(.bss)
        *(.bss*)
        _kernelend = . - vma;
        _kernelvend = .;
    }

    .init_array : AT(ADDR(.init_array) - vma) {
        *(.init_array*)
    }

    /DISCARD/ : { 
        *(.note.gnu.build-id*) 
        *(.eh_frame*)
        *(.debug_info*)
        *(.debug_abbrev*)
        *(.debug_aranges*)
        *(.debug_line*)
        *(.debug_str*)
        *(.debug_line_str*)
        *(.comment*)
    }

    # .note.gnu.build-id : { *(.note.gnu.build-id*) } >mem0
    # .eh_frame          : { *(.eh_frame*) } >mem0
    # .eh_frame_hdr      : { *(.eh_frame_hdr*) } >mem0
    # .debug_info        : { *(.debug_info*) } >mem0
    # .debug_abbrev      : { *(.debug_abbrev*) } >mem0
    # .debug_aranges     : { *(.debug_aranges*) } >mem0
    # .debug_line        : { *(.debug_line*) } >mem0
    # .debug_str         : { *(.debug_str*) } >mem0
    # .debug_line_str    : { *(.debug_line_str*) } >mem0
    # .comment           : { *(.comment*) } >mem0
}