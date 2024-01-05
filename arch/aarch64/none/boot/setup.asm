.global aarch64_faultjmp
.extern aarch64_fault

.section .boot.text

.macro bong
.balign 0x80
stp	x29, x30, [sp, #-48]!
mov	x29, sp
bl aarch64_fault
ldp	x29, x30, [sp], #48
ret
.endm

.balign 0x800
aarch64_faultjmp:
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong
    bong