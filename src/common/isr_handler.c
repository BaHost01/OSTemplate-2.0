#include "idt.h"
#include "vga.h"

void isr_handler(struct registers* r) {
    vga_puts("Received interrupt: ");
    if (r->int_no == 0) {
        vga_puts("Division by Zero\n");
    } else if (r->int_no == 1) {
        vga_puts("Debug Exception\n");
    } else {
        vga_puts("Unknown Exception\n");
    }
}
