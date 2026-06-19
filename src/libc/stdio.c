#include "stdio.h"
#include "syscall.h"
#include "string.h"

int putchar(int c) {
    char ch = (char)c;
    sys_write(1, &ch, 1);
    return c;
}

int puts(const char *s) {
    int len = strlen(s);
    sys_write(1, s, len);
    putchar('\n');
    return len + 1;
}

static void print_uint(unsigned int val, int base) {
    char buf[32];
    int i = 0;
    if (val == 0) {
        putchar('0');
        return;
    }
    while (val > 0) {
        int rem = val % base;
        buf[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
        val /= base;
    }
    while (i > 0) {
        putchar(buf[--i]);
    }
}

static void print_int(int val) {
    if (val < 0) {
        putchar('-');
        print_uint(-val, 10);
    } else {
        print_uint(val, 10);
    }
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                print_int(va_arg(args, int));
            } else if (*format == 'u') {
                print_uint(va_arg(args, unsigned int), 10);
            } else if (*format == 'x') {
                print_uint(va_arg(args, unsigned int), 16);
            } else if (*format == 's') {
                const char *s = va_arg(args, const char *);
                sys_write(1, s, strlen(s));
            } else if (*format == 'c') {
                putchar(va_arg(args, int));
            } else if (*format == '%') {
                putchar('%');
            }
        } else {
            putchar(*format);
        }
        format++;
    }

    va_end(args);
    return 0; // Simple implementation, we don't count chars yet
}
