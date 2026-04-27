#include "stdio.h"
#include "vga.h"
#include "serial.h"
#include "string.h"

static char* itoa(uint64_t val, int base) {
    static char buf[65];
    int i = 64;
    buf[i] = '\0';
    if (val == 0) buf[--i] = '0';
    while (val > 0) {
        int rem = val % base;
        buf[--i] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        val /= base;
    }
    return &buf[i];
}

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 's') {
                char* s = va_arg(args, char*);
                vga_puts(s);
                serial_puts(s);
            } else if (*fmt == 'd') {
                uint64_t d = va_arg(args, uint64_t);
                char* s = itoa(d, 10);
                vga_puts(s);
                serial_puts(s);
            } else if (*fmt == 'x' || *fmt == 'p') {
                uint64_t x = va_arg(args, uint64_t);
                char* s = itoa(x, 16);
                vga_puts("0x");
                vga_puts(s);
                serial_puts("0x");
                serial_puts(s);
            }
        } else {
            vga_putc(*fmt);
            serial_putc(*fmt);
        }
        fmt++;
    }
    va_end(args);
}
