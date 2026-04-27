global gdt_flush
gdt_flush:
    lgdt [rdi]
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; 64-bit far return (trick to reload CS)
    push 0x08
    lea rax, [rel .flush]
    push rax
    retfq

.flush:
    ret
