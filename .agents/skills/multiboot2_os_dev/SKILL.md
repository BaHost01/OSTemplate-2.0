---
name: multiboot2_os_dev
description: Guidelines and structures for Multiboot2 and x86_64 Long Mode early boot configurations.
---

# Multiboot2 & x86_64 Long Mode OS Development Skill

This skill provides templates, structure definitions, and compilation checks for working on the SimpleOS early boot process.

## 1. Multiboot2 Header Reference (NASM)

Ensure your early assembly header is structured as follows, keeping alignment constraints:

```assembly
; Multiboot 2 Header Configuration
MB2_MAGIC    equ 0xe85250d6
MB2_ARCH     equ 0 ; i386 protected mode
MB2_LENGTH   equ multiboot2_header_end - multiboot2_header_start
MB2_CHECKSUM equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

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
```

## 2. 64-Bit Transition Checklist

To transition from 32-bit Multiboot to 64-bit Long Mode:
1. **Disable Interrupts**: `cli`
2. **Paging structures**: Define `pml4`, `pdp`, and `pd` in BSS aligned to 4096 bytes.
3. **Map Memory**: Identity map at least the first 512MB (using a loop over 256 entries in PD with 2MB huge pages).
4. **Enable PAE**: Set bit 5 in CR4.
5. **Load CR3**: Write physical address of PML4 to CR3.
6. **Enable Long Mode**: Set bit 8 in EFER MSR (`0xC0000080`).
7. **Enable Paging**: Set bit 31 in CR0.
8. **Load 64-bit GDT**: Load GDT with 64-bit code and data descriptor selectors via `lgdt`.
9. **Far Jump**: Jump to 64-bit entry point to load CS selector.

## 3. C Runtime Tag Parsing

```c
struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot2_tag_framebuffer {
    struct multiboot2_tag tag;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
};

// Iteration template:
void parse_mbi(void* info) {
    struct multiboot2_tag* tag;
    for (tag = (struct multiboot2_tag*)((uint8_t*)info + 8);
         tag->type != 0;
         tag = (struct multiboot2_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        if (tag->type == 8) { // Framebuffer tag
            struct multiboot2_tag_framebuffer* fb = (struct multiboot2_tag_framebuffer*)tag;
            // Initialize framebuffer
        }
    }
}
```
