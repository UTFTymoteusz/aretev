%include "arch/x86_64/none/cpu/const.asm"

global _start

extern multiboot_start
extern init_earlyidt
extern vma

PROT_PRESENT equ 1 << 0
PROT_WRITE   equ 1 << 1
PROT_LARGE   equ 1 << 7

MMAP_1KiB  equ 0
MMAP_2MiB  equ 1
MMAP_GUARD equ 2

section .boot.text
align 16
bits 32

longmode_msg:
  db "glupi 32 bitowy procek :(", 0

_start:
    mov esp, stack.end

    push eax
    push ebx

    call longmode_guard
    call paging_init

    mov ecx, IA32_EFER
    rdmsr
    or eax, LME
    wrmsr

    mov eax, cr0
    or  eax, PG
    and eax, ~(NW | CD)
    mov cr0, eax

    pop ebx
    pop eax

	  lgdt [bare_gdt.ptr]
    jmp bare_gdt.code:_start64

longmode_guard:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb  longmode_fail

    mov  eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz   longmode_fail

    ret

longmode_fail:
    xor esi, esi
    xor edi, edi
    mov ebx, 0x000B8000

  .loop_clr:
    mov byte [ebx + edi * 2 + 0], 0
    mov byte [ebx + edi * 2 + 1], 0x07
    inc edi
    cmp edi, 2000
    jne .loop_clr

    xor edi, edi
    
    mov al, 0x0A
    mov dx, 0x3D4
    out dx, al
    mov al, 0x20
    mov dx, 0x3D5
    out dx, al

  .loop_msg:
    mov al, longmode_msg[esi]
    cmp al, 0
    je  .end
    mov byte [ebx + edi * 2 + 0], al
    mov byte [ebx + edi * 2 + 1], 0x1F
    inc esi
    inc edi
    jmp .loop_msg

  .end:
    cli
    hlt

paging_init:
    push ebp
    mov ebp, esp

    call paging_zero
    call paging_link

	  mov eax, cr4
    or  eax, PGE | PAE | PSE
    mov cr4, eax

    mov eax, pml4t
    mov cr3, eax

    pop ebp
    ret

paging_zero:
    push ebp
    mov ebp, esp

    push dword 4096
    push dword 0
    push dword pml4t
    call memset
    add  esp, 12

    push dword 4096
    push dword 0
    push dword pdpt
    call memset
    add  esp, 12

    push dword 4096
    push dword 0
    push dword pdt
    call memset
    add  esp, 12

    pop ebp
    ret

paging_link:
    push ebp
    mov ebp, esp

    mov eax, pdpt
    or  eax, PROT_WRITE | PROT_PRESENT
    mov dword [pml4t + 0], eax
    mov dword [pml4t + 4], 0x00000000

    mov eax, pdt
    or  eax, PROT_WRITE | PROT_PRESENT
    mov dword [pdpt + 0], eax
    mov dword [pdpt + 4], 0x00000000

    mov eax, pt
    or  eax, PROT_WRITE | PROT_PRESENT
    mov dword [pdt + 0], eax
    mov dword [pdt + 4], 0x00000000

    mov eax, pt
    mov ebx, PROT_WRITE | PROT_PRESENT

  .setloop:
    mov dword [eax + 0], ebx
    mov dword [eax + 4], 0x00000000
    add eax, 8
    add ebx, 0x1000
    cmp eax, pt + 0x0800
    jl .setloop

  .zeroloop:
    mov dword [eax + 0], 0x00000000
    mov dword [eax + 4], 0x00000000
    add eax, 8
    cmp eax, pt + 0x1000
    jl .zeroloop

    pop ebp
    ret

; A C-like memset.
; arg0 - Target pointer.
; arg1 - Replacement value.
; arg2 - Target length in bytes.
memset:
    push ebp
    mov ebp, esp

    mov dword edi, [ebp + 8]
    mov dword eax, [ebp + 12]
    mov dword ecx, [ebp + 16]

    rep stosb

    xor eax, eax
    pop ebp
    ret

bits 64

_start64:
    push rax
    push rbx

    mov rax, cr0
    and rax, ~EM
    or  rax, MP
    mov rax, cr0

    mov rax, cr4
    or  rax, OSXMMEXCPT | OSFXSR
    mov cr4, rax

    mov ax, bare_gdt.data
    mov ds, ax
    mov es, ax
    mov ss, ax

    xor rax, rax
    mov gs, rax
    mov fs, rax

    call init_earlyidt

    mov  rdi, vma
    mov  rsi, 0x00000000000
    mov  rdx, MMAP_2MiB
    call paging_map

    mov  rdi, stack.guard
    xor  rsi, rsi
    mov  rdx, MMAP_GUARD
    call paging_map

    ; Page table recusive mapping
    mov rax, cr3
    mov rbx, cr3
    or  rbx, PROT_WRITE | PROT_PRESENT
    mov [rax + 0x1F4 * 8], rbx

    pop rbx
    pop rax

    ; multiboot check
    cmp eax, 0x2BADB002
    je .multiboot

  .multiboot:
    mov rdi, rbx
    call multiboot_start

; Allocates memory from the miniheap and initializes it to 0.
; rdi - Allocation size in bytes.
; Returns: rax - Pointer to the allocated memory.
minimalloc:
    push rbp
    mov rbp, rsp
    push rbx

    mov qword rcx, [miniheap_pointer]
    mov rbx, rcx
    add qword [miniheap_pointer], rdi

    ; Can't call memset, because it's a 32-bit function.
    mov qword rcx, rdi
    xor rax, rax
    mov qword rdi, rbx

    rep stosb

    mov rax, rbx

    pop rbx
    pop rbp
    ret

; Maps a page to a physical location.
; rdi - Virtual address
; rsi - Physical address
; rdx - Mapping type (0 - 4 KiB, 1 - 2 MiB, 2 - Stack guard)
paging_map:
    push rbp
    mov rbp, rsp

    mov rbx, rdi ; rbx - Virtual address
    mov r9 , rdx ; r9  - 1 if we want 2 MiB page

    mov rcx, rbx ; pml4t zone
    shr rcx, 48 - 9
    and rcx, (1 << 9) - 1
    mov qword rdx, [pml4t + rcx * 8]
    mov r8, pml4t
    cmp rdx, 0x0000000000000000
    jne .pdpt_exists

    push rcx
    mov  rdi, 4096
    call minimalloc
    pop  rcx

    mov rdx, rax
    or  rdx, PROT_WRITE | PROT_PRESENT
    mov qword [pml4t + rcx * 8], rdx

    mov rdx, rax

  .pdpt_exists:
    and rdx, ~((1 << 12) - 1)
    mov r8 , rdx
    mov rcx, rbx ; pdpt zone
    shr rcx, 48 - 18
    and rcx, (1 << 9) - 1
    mov qword rdx, [r8 + rcx * 8]
    cmp rdx, 0x0000000000000000
    jne .pdt_exists

    push rcx
    mov  rdi, 4096
    call minimalloc
    pop  rcx

    mov rdx, rax
    or  rdx, PROT_WRITE | PROT_PRESENT
    mov qword [r8 + rcx * 8], rdx

    mov rdx, rax

  .pdt_exists:
    cmp r9, 1
    je .largepages

    and rdx, ~((1 << 12) - 1)
    mov r8 , rdx
    mov rcx, rbx ; pdt zone
    shr rcx, 48 - 27
    and rcx, (1 << 9) - 1
    mov qword rdx, [r8 + rcx * 8]
    cmp rdx, 0x0000000000000000
    jne .pt_exists

    push rcx
    mov  rdi, 4096
    call minimalloc
    pop  rcx

    mov rdx, rax
    or  rdx, PROT_WRITE | PROT_PRESENT
    mov qword [r8 + rcx * 8], rdx

    mov rdx, rax

  .pt_exists:
    and rdx, ~((1 << 12) - 1)
    mov r8 , rdx
    mov rcx, rbx ; pdt zone
    shr rcx, 48 - 36
    and rcx, (1 << 9) - 1

    cmp r9, 2
    je .guard

    or  rsi, PROT_WRITE | PROT_PRESENT
    mov qword [r8 + rcx * 8], rsi

    pop rbp
    ret

  .guard:
    mov qword [r8 + rcx * 8], 0x0000000054650000

    invlpg [rbx]

    pop rbp
    ret

  .largepages:
    and rdx, ~((1 << 12) - 1)
    mov r8 , rdx
    mov rcx, rbx ; pdt zone
    shr rcx, 48 - 27
    and rcx, (1 << 9) - 1

    or  rsi, PROT_LARGE | PROT_WRITE | PROT_PRESENT
    mov qword [r8 + rcx * 8], rsi

    pop rbp
    ret

section .boot.data
align 16

miniheap_pointer:
    dq miniheap

bare_gdt:
  .null: equ $ - bare_gdt
	dw 0xFFFF     ; Limit
	dw 0x00       ; Base (low)
	db 0x00       ; Base (middle)
	db 0x00       ; Access
	db 0x01       ; Granularity
	db 0x00       ; Base (high)
  .code: equ $ - bare_gdt
	dw 0xFFFF     ; Limit
	dw 0x00       ; Base (low)
	db 0x00       ; Base (middle)
	db 0b10011010 ; Access (exec/read)
	db 0b10101111 ; Granularity, 64 bits, limit 19:16
	db 0x00       ; Base (high)
  .data: equ $ - bare_gdt
	dw 0xFFFF     ; Limit
	dw 0x00       ; Base (low)
	db 0x00       ; Base (middle)
	db 0b10010010 ; Access (read/write)
	db 0b10000000 ; Granularity
	db 0x00       ; Base (high)
  .ptr:
	dw $ - bare_gdt - 1 ; Limit
	dq bare_gdt         ; Base

section .boot.bss nobits
align 4096

pml4t:
    resq 512
pdpt:
    resq 512
pdt:
    resq 512
pt:
    resq 512

align 4096
miniheap:
    resb 16384
stack:
  .guard:
    resb 4096
  resb 8192 * 2
  .end: equ $