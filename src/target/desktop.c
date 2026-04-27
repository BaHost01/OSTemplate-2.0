#include "desktop.h"
#include "../common/vga.h"
#include "../common/keyboard.h"
#include "../common/string.h"
#include "edit.h"
#include "pkg.h"

void draw_taskbar() {
    vga_set_color(0x01, 0x07); // Blue on Light Grey
    for(int i = 0; i < 80; i++) {
        // Draw at the bottom row (24)
        // Note: vga_putc doesn't support specific coordinates yet, 
        // we'd need a vga_set_cursor(x, y)
    }
}

void desktop_init() {
    vga_clear();
    vga_set_color(0x0F, 0x01); // White on Blue (Classic Professional feel)
    vga_clear();
}

void desktop_show() {
    vga_clear();
    vga_set_color(0x0F, 0x01); // Background
    vga_puts("================================================================================\n");
    vga_puts("  MyOS x64 Ultimate - Professional Desktop Environment                         \n");
    vga_puts("================================================================================\n");
    vga_puts("\n");
    vga_puts("  [F1] File Explorer   [F2] Text Editor   [F3] Package Manager   [F4] Terminal \n");
    vga_puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    vga_puts("--------------------------------------------------------------------------------\n");
    vga_puts("  [ Start ]                                                     12:00 PM       \n");
    
    while(1) {
        char c = keyboard_get_char();
        if (c == '1') { // Simulated F1
            vga_clear();
            vga_puts("Opening File Explorer...\n");
            // explorer_main();
            break;
        } else if (c == '2') { // Simulated F2
            editor_main();
            desktop_show(); // Redraw after exit
        } else if (c == '4') {
            break; // Exit to Shell
        }
    }
}
