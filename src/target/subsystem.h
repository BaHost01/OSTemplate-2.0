#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <stdint.h>
#include "../common/idt.h"

typedef enum {
    SUBSYSTEM_NATIVE,
    SUBSYSTEM_LINUX,
    SUBSYSTEM_WINDOWS
} subsystem_t;

void subsystem_init();
void handle_linux_syscall(struct registers* r);
void handle_win32_syscall(struct registers* r);

#endif
