#include "object_manager.h"
#include "../common/string.h"
#include "../common/kheap.h"
#include "../common/vga.h"

static handle_entry_t handle_table[MAX_HANDLES];
static uint32_t next_handle = 1;

void ob_init() {
    memset(handle_table, 0, sizeof(handle_table));
    vga_puts("Windows NT Object Manager (Simple) Initialized\n");
}

uint32_t ob_create_handle(void* object, object_type_t type) {
    if (next_handle >= MAX_HANDLES) return 0;
    
    uint32_t h = next_handle++;
    handle_table[h].handle_id = h;
    handle_table[h].kernel_object = object;
    handle_table[h].type = type;
    
    return h;
}

void* ob_reference_object_by_handle(uint32_t handle) {
    if (handle >= MAX_HANDLES) return NULL;
    return handle_table[handle].kernel_object;
}
