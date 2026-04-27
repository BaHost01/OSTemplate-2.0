#ifndef KSOD_H
#define KSOD_H

#include "../common/idt.h"

void ksod_trigger(struct registers* r, const char* reason);

#endif
