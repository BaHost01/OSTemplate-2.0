#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <stdarg.h>
#include <stddef.h>

int printf(const char *format, ...);
int puts(const char *s);
int putchar(int c);

#endif
