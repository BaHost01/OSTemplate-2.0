#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdint.h>

typedef struct {
    char key[64];
    uint32_t value;
} registry_entry_t;

void reg_set_int(const char* key, uint32_t val);
uint32_t reg_get_int(const char* key, uint32_t default_val);

#endif
