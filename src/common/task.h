#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct {
    uint64_t id;
    uint64_t rip;
    uint64_t rsp;
    uint64_t cr3;
    // For a real scheduler, we'd add state, registers, etc.
} task_t;

void task_init();
void task_exec(const char *filename);

#endif
