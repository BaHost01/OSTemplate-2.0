#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

void paging_init();
void paging_switch_directory(uint32_t* dir);

#endif
