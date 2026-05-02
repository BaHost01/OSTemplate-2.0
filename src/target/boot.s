; Multiboot 1 Header
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ (1 << 0) | (1 << 1)
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

; Multiboot 2 Header
MB2_MAGIC    equ 0xe85250d6
MB2_ARCH     equ 0 ; i386 protected mode
MB2_LENGTH   equ multiboot2_header_end - multiboot2_header_start
MB2_CHECKSUM equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .multiboot2
align 8
multiboot2_header_start:
    dd MB2_MAGIC
    dd MB2_ARCH
    dd MB2_LENGTH
    dd MB2_CHECKSUM
    
    ; Request Framebuffer Tag (Type 5)
    align 8
    dw 5 ; type
    dw 0 ; flags
    dd 20 ; size
    dd 1024 ; width
    dd 768 ; height
    dd 32 ; depth
    
    ; End tag
    align 8
    dw 0
    dw 0
    dd 8
multiboot2_header_end:

section .bss
align 4096
pml4:
    resb 4096
pdp:
    resb 4096
pd:
    resb 4096
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

multiboot_magic:
    resd 1
multiboot_info:
    resd 1

section .text
[bits 32]
global _start
_start:
    ; Disable interrupts
    cli

    ; Store multiboot magic and info
    mov [multiboot_magic], eax
    mov [multiboot_info], ebx

    ; Set up stack for 32-bit transition
    mov esp, stack_top

    ; 1. Set up paging for long mode transition
    ; Identity map the first 512MB using 2MB huge pages
    
    ; Clear PML4
    mov ecx, 1024
    xor eax, eax
    mov edi, pml4
    rep stosd
    
    ; Clear PDP
    mov ecx, 1024
    xor eax, eax
    mov edi, pdp
    rep stosd

    ; Clear PD
    mov ecx, 1024
    xor eax, eax
    mov edi, pd
    rep stosd

    ; PML4[0] -> PDP
    mov eax, pdp
    or eax, 0x3 ; Present | Write
    mov [pml4], eax

    ; PDP[0] -> PD
    mov eax, pd
    or eax, 0x3 ; Present | Write
    mov [pdp], eax

    ; Map first 512MB using 2MB huge pages
    mov ecx, 256
    mov eax, 0x83 ; Present | Write | Huge
    mov edi, pd
.map_loop:
    mov [edi], eax
    add eax, 0x200000
    add edi, 8
    loop .map_loop

    ; 2. Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; 3. Load CR3 with PML4
    mov eax, pml4
    mov cr3, eax

    ; 4. Enable Long Mode in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; 5. Enable Paging and Protected Mode
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; 6. Load 64-bit GDT
    lgdt [gdt64_ptr]

    ; 7. Long jump to 64-bit mode
    push 0x08
    push long_mode_start
    retf

[bits 64]
long_mode_start:
    ; Set up segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up 64-bit stack
    mov rsp, stack_top

    ; Restore multiboot magic and info
    mov edi, [multiboot_magic]
    mov esi, [multiboot_info]
    
    extern kmain
    call kmain

    cli
.hang:
    hlt
    jmp .hang

section .rodata
align 8
gdt64:
    dq 0 ; null
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code descriptor
    dq (1<<44) | (1<<47) | (1<<41) ; data descriptor
gdt64_ptr:
    dw $ - gdt64 - 1
    dq gdt64
