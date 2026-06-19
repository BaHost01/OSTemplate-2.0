#ifndef INITRD_H
#define INITRD_H

#include <stdint.h>
#include <stddef.h>

void initrd_init(uint64_t addr, uint32_t size);
const uint8_t* initrd_get_file(const char* filename, size_t* size);

#endif
