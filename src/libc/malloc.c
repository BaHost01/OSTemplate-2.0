#include <stddef.h>
#include "syscall.h"

// A very simple static bump allocator for now, until we implement sys_brk
static char heap[1024 * 1024]; // 1MB heap
static size_t heap_offset = 0;

void *malloc(size_t size) {
    if (heap_offset + size > sizeof(heap)) {
        return NULL; // Out of memory
    }
    void *ptr = &heap[heap_offset];
    heap_offset += size;
    return ptr;
}

void free(void *ptr) {
    // Basic bump allocator doesn't support freeing
    (void)ptr;
}
