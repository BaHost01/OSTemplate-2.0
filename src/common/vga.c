#include "vga.h"
#include "io.h"
#include "string.h"

static uint16_t* vga_buffer = (uint16_t*)0xB8000;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t vga_row;
static size_t vga_col;
static uint8_t vga_color;

static void update_cursor() {
    uint16_t pos = vga_row * VGA_WIDTH + vga_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_init() {
    vga_row = 0;
    vga_col = 0;
    vga_color = 0x07;
    vga_clear();
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_color = fg | (bg << 4);
}

void vga_clear() {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;
    update_cursor();
}

static void scroll() {
    for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        vga_buffer[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = VGA_HEIGHT - 1;
}

void vga_putc(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
    } else if (c == '\b') {
        if (vga_col > 0) {
            vga_col--;
            const size_t index = vga_row * VGA_WIDTH + vga_col;
            vga_buffer[index] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        }
    } else if (c == '\r') {
        vga_col = 0;
    } else {
        const size_t index = vga_row * VGA_WIDTH + vga_col;
        vga_buffer[index] = (uint16_t)c | ((uint16_t)vga_color << 8);
        vga_col++;
    }

    if (vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
    }

    if (vga_row >= VGA_HEIGHT) {
        scroll();
    }
    update_cursor();
}

void vga_puts(const char* s) {
    while (*s) {
        vga_putc(*s++);
    }
}
