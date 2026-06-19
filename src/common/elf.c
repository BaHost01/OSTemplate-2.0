#include "elf.h"
#include "string.h"
#include "stdio.h"
#include "paging.h"

// Note: This basic loader assumes the memory ranges required by the ELF
// are already mapped or we are identity mapping. A real implementation
// would parse PT_LOAD and allocate/map memory for the process.

uint64_t elf_load_from_memory(const uint8_t *binary) {
    elf64_header *header = (elf64_header *)binary;

    if (header->e_magic != ELF_MAGIC) {
        kprintf("[ELF] Invalid magic number\n");
        return 0;
    }

    if (header->e_type != 2) { // ET_EXEC
        kprintf("[ELF] Not an executable file\n");
        return 0;
    }

    elf64_phdr *phdrs = (elf64_phdr *)(binary + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (phdrs[i].p_type == PT_LOAD) {
            uint8_t *dest = (uint8_t *)phdrs[i].p_vaddr;
            const uint8_t *src = binary + phdrs[i].p_offset;
            
            // Map memory (this is a simplified placeholder, normally you'd call
            // a virtual memory manager to allocate physical pages and map to p_vaddr)
            
            // Copy file data
            memcpy(dest, src, phdrs[i].p_filesz);
            
            // Zero out remaining memory (bss)
            if (phdrs[i].p_memsz > phdrs[i].p_filesz) {
                memset(dest + phdrs[i].p_filesz, 0, phdrs[i].p_memsz - phdrs[i].p_filesz);
            }
        }
    }

    return header->e_entry;
}
