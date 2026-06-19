#include "task.h"
#include "initrd.h"
#include "elf.h"
#include "stdio.h"
#include "kheap.h"
#include "string.h"

extern void jump_to_user(uint64_t entry_point, uint64_t stack_ptr);

void task_init() {
    kprintf("[Task] Task manager initialized\n");
}

void task_exec(const char *filename) {
    size_t file_size = 0;
    
    // Check if path has /bin/ prefix and strip it for initrd search
    // (since our simple initrd might just have 'sh' instead of '/bin/sh')
    const char *search_name = filename;
    if (strncmp(filename, "/bin/", 5) == 0) {
        search_name = filename + 5;
    }
    
    const uint8_t *binary = initrd_get_file(search_name, &file_size);
    
    if (!binary) {
        // Try without stripping just in case
        binary = initrd_get_file(filename, &file_size);
    }
    
    if (!binary) {
        kprintf("[Task] Executable not found: %s\n", filename);
        return;
    }
    
    uint64_t entry = elf_load_from_memory(binary);
    if (!entry) {
        kprintf("[Task] Failed to load ELF: %s\n", filename);
        return;
    }
    
    // Allocate a basic user stack
    // In a real OS, this would be a properly mapped page in Ring 3
    // For our minimal bridge, we'll just allocate a chunk of memory
    // WARNING: This memory is Ring 0 in identity mapping, 
    // so user mode (Ring 3) will page fault if CR3 isn't set up 
    // to allow user access to this region.
    // 
    // To make this work instantly without a full page fault handler:
    // We assume `jump_to_user` works because the segment limits are 0-4GB
    // But x86_64 paging also enforces user/supervisor bits.
    // Ensure `paging_init` maps the first few MBs with User (U/S = 1) access.
    
    uint8_t *user_stack = (uint8_t *)kmalloc(16384); 
    uint64_t rsp = (uint64_t)(user_stack + 16384 - 8);
    
    kprintf("[Task] Jumping to user mode: %s at %p (stack %p)\n", filename, (void*)entry, (void*)rsp);
    
    jump_to_user(entry, rsp);
    
    // We should never return here if jump_to_user succeeds
}
