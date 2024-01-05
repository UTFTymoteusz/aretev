.global _start
.section .boot.text

# x0 is the dtb address apparently
_start:
    ldr x30, =stack.end
    mov sp, x30

    sub sp, sp, 16
    stp x0, x1, [sp, #16]

    bl aarch64_setup

    ldp x0, x1, [sp, #16]
    add sp, sp, 16

    bl baremetal_start

.section .boot.bss
stack:
    .space 8192 * 2
stack.end: .long .
