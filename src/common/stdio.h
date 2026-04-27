#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stddef.h>

void kprintf(const char* fmt, ...);
int sprintf(char* buf, const char* fmt, ...);

#endif
