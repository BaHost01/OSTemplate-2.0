#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    OBJ_TYPE_FILE,
    OBJ_TYPE_PROCESS,
    OBJ_TYPE_THREAD,
    OBJ_TYPE_EVENT,
    OBJ_TYPE_MUTANT
} object_type_t;

typedef struct {
    uint32_t handle_id;
    object_type_t type;
    void* kernel_object;
    uint32_t access_mask;
} handle_entry_t;

#define MAX_HANDLES 1024

void ob_init();
uint32_t ob_create_handle(void* object, object_type_t type);
void* ob_reference_object_by_handle(uint32_t handle);

#endif
