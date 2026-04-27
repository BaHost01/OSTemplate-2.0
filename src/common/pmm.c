#include "pmm.h"
#include "string.h"

static uint32_t* pmm_bitmap;
static uint32_t  pmm_max_blocks;
static uint32_t  pmm_used_blocks;

void pmm_init(uint32_t mem_size) {
    pmm_max_blocks = mem_size / PAGE_SIZE;
    pmm_used_blocks = pmm_max_blocks;
    
    // We'll place the bitmap at 4MB for now (just an example)
    pmm_bitmap = (uint32_t*)0x400000;
    memset(pmm_bitmap, 0xFF, pmm_max_blocks / 8);
}

void pmm_init_region(uint32_t base, uint32_t size) {
    uint32_t align = base / PAGE_SIZE;
    uint32_t blocks = size / PAGE_SIZE;
    
    for (; blocks > 0; blocks--) {
        pmm_bitmap[align / 32] &= ~(1 << (align % 32));
        pmm_used_blocks--;
        align++;
    }
}

void* pmm_alloc_block() {
    for (uint32_t i = 0; i < pmm_max_blocks / 32; i++) {
        if (pmm_bitmap[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (!(pmm_bitmap[i] & (1 << j))) {
                    pmm_bitmap[i] |= (1 << j);
                    pmm_used_blocks++;
                    return (void*)((i * 32 + j) * PAGE_SIZE);
                }
            }
        }
    }
    return NULL;
}

void pmm_free_block(void* p) {
    uint32_t addr = (uint32_t)p;
    uint32_t block = addr / PAGE_SIZE;
    pmm_bitmap[block / 32] &= ~(1 << (block % 32));
    pmm_used_blocks--;
}
