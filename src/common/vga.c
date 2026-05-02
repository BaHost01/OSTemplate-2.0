#include "vga.h"
#include "multiboot.h"
#include "string.h"

static uint32_t* fb_addr = NULL;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t fb_pitch = 0;
static uint8_t fb_bpp = 0;

static size_t cursor_x = 0;
static size_t cursor_y = 0;
static uint32_t fg_color = 0xFFFFFFFF; // White
static uint32_t bg_color = 0x00000000; // Black

// Basic 8x16 font (simplified from Linux font_8x16.c)
extern unsigned char font_8x16[4096];

static void putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb_width || y >= fb_height) return;
    fb_addr[y * (fb_pitch / 4) + x] = color;
}

static void draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg) {
    for (int i = 0; i < 16; i++) {
        uint8_t row = font_8x16[(uint8_t)c * 16 + i];
        for (int j = 0; j < 8; j++) {
            if (row & (0x80 >> j)) {
                putpixel(x + j, y + i, fg);
            } else {
                putpixel(x + j, y + i, bg);
            }
        }
    }
}

void vga_init_fb(struct multiboot2_tag_framebuffer* tag) {
    fb_addr = (uint32_t*)tag->framebuffer_addr;
    fb_width = tag->framebuffer_width;
    fb_height = tag->framebuffer_height;
    fb_pitch = tag->framebuffer_pitch;
    fb_bpp = tag->framebuffer_bpp;
    vga_clear();
}

void vga_init() {
    // Legacy init, should ideally be replaced by vga_init_fb
    // For now we keep it to avoid breaking compilation if kmain isn't updated
}

void vga_clear() {
    if (!fb_addr) return;
    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            putpixel(x, y, bg_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putc(char c) {
    if (!fb_addr) return;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += 16;
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        draw_char(c, cursor_x, cursor_y, fg_color, bg_color);
        cursor_x += 8;
    }

    if (cursor_x >= fb_width) {
        cursor_x = 0;
        cursor_y += 16;
    }

    if (cursor_y + 16 >= fb_height) {
        // Scrolling implementation could be added here
        vga_clear(); // Simple clear for now
    }
}

void vga_puts(const char* s) {
    while (*s) {
        vga_putc(*s++);
    }
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    // Basic mapping from 4-bit VGA colors to 32-bit RGB
    uint32_t colors[] = {
        0x00000000, 0x000000AA, 0x0000AA00, 0x0000AAAA,
        0x00AA0000, 0x00AA00AA, 0x00AA5500, 0x00AAAAAA,
        0x00555555, 0x005555FF, 0x0055FF55, 0x0055FFFF,
        0x00FF5555, 0x00FF55FF, 0x00FFFF55, 0x00FFFFFF
    };
    fg_color = colors[fg & 0x0F];
    bg_color = colors[bg & 0x0F];
}
