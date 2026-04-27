; Multiboot 1 Header
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ (1 << 0) | (1 << 1)
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

section .text
[bits 64]
global _start:function (_start.end - _start)
_start:
    mov rsp, stack_top
    
    extern kmain
    mov rdi, rax ; magic
    mov rsi, rbx ; info
    call kmain

    cli
.hang:
    hlt
    jmp .hang
.end:
