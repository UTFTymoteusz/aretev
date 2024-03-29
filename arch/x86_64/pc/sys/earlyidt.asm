[BITS 64]

global init_earlyidt
extern early_fault_handler

section .boot.text

%macro flt_err 1
    flt%1:
        push qword %1
        jmp earlyidt_handler
%endmacro

%macro flt_noerr 1
    flt%1:
        push qword 0
        push qword %1
        jmp earlyidt_handler
%endmacro

%macro flt_entry 1
    dq flt%1
%endmacro

flt_array:
    %assign i 0
    %rep 32
        flt_entry i
        %assign i i + 1
    %endrep

flt_noerr  0
flt_noerr  1
flt_noerr  2
flt_noerr  3
flt_noerr  4
flt_noerr  5
flt_noerr  6
flt_noerr  7
flt_err    8
flt_noerr  9
flt_err   10
flt_err   11
flt_err   12
flt_err   13
flt_err   14
flt_noerr 15
flt_noerr 16
flt_noerr 17
flt_noerr 18
flt_noerr 19
flt_noerr 20
flt_noerr 21
flt_noerr 22
flt_noerr 23
flt_noerr 24
flt_noerr 25
flt_noerr 26
flt_noerr 27
flt_noerr 28
flt_noerr 29
flt_noerr 30
flt_noerr 31

init_earlyidt:
	push rbp
	mov rbp, rsp

	mov r8, early_idt
	xor r9, r9
    mov r12, flt_array

	.loop:
        mov qword r10, [r12]

		; First part of the offset
		mov r11, r10
		and r11, 0xFFFF
		mov word [r8 + 0x00], r11w

		; Code selector
		mov word [r8 + 0x02], 0x08

		; IST
		mov byte [r8 + 0x04], 0x00

		; Type and attributes
		mov byte [r8 + 0x05], 0x8E

		; Second part of the offset
		mov r11, r10
		shr r11, 16
		and r11, 0xFFFF
		mov word [r8 + 0x06], r11w

		; Third part of the offset
		mov r11, r10
		shr r11, 32
		and r11, 0xFFFFFFFF
		mov dword [r8 + 0x08], r11d

		; Zero
		xor rax, rax
		mov dword [r8 + 0x0C], eax

		add r8 , 16
        add r12, 8
		inc r9
		cmp r9, 32
		jl .loop

	lidt [early_idt_ptr]

	pop rbp
	ret

earlyidt_handler:
    pop rbx ; Exception ID
	pop rcx ; Code

	mov r15, [rsp]

	mov dx, 0x3F8
	mov al, 'e'
	out dx, al
	mov al, 'x'
	out dx, al
	mov al, 'c'
	out dx, al
	mov al, ' '
	out dx, al

	mov ax, bx
	idiv byte [early_idt_divisor]

	add al, '0'
	add ah, '0'

	out dx, al
	mov al, ah
	out dx, al

	mov al, 13
	out dx, al
	mov al, 10
	out dx, al
	
	hlt
	
section .boot.data
early_idt_ptr:
	dw 32 * 16 - 1
	dq early_idt

early_idt_divisor:
	db 10

section .boot.bss nobits
early_idt:
	resb 32 * 16
