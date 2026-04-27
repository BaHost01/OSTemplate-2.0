#include "kheap.h"

#define KHEAP_SIZE 4194304 // 4MB
static uint8_t kheap_pool[KHEAP_SIZE];
static size_t kheap_ptr = 0;

void kheap_init() {
    kheap_ptr = 0;
}

void* kmalloc(size_t size) {
    if (kheap_ptr + size > KHEAP_SIZE) {
        return NULL; // Out of memory
    }
    void* ptr = &kheap_pool[kheap_ptr];
    kheap_ptr += size;
    
    // Simple 4-byte alignment
    if (kheap_ptr % 4 != 0) {
        kheap_ptr += (4 - (kheap_ptr % 4));
    }
    
    return ptr;
}

void kfree(void* ptr) {
    // Bump allocator doesn't support free, but we can implement a reset later if needed
    (void)ptr;
}
