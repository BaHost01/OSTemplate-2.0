#include "edit.h"
#include "../common/vga.h"
#include "../common/keyboard.h"
#include "../common/string.h"
#include "../common/kheap.h"

#define MAX_ROWS 24
#define MAX_COLS 80

typedef struct {
    char lines[MAX_ROWS][MAX_COLS];
    int cx, cy;
} EditorState;

void editor_draw(EditorState* es) {
    vga_clear();
    for (int y = 0; y < MAX_ROWS; y++) {
        for (int x = 0; x < MAX_COLS; x++) {
            if (es->lines[y][x] == '\0') break;
            // Manual character placement could be added here if vga_putc isn't enough
            vga_putc(es->lines[y][x]);
        }
        vga_putc('\n');
    }
}

void editor_main() {
    EditorState* es = (EditorState*)kmalloc(sizeof(EditorState));
    memset(es, 0, sizeof(EditorState));
    
    vga_clear();
    vga_puts("--- SimpleOS Editor (Nano-lite) ---\n");
    vga_puts("Press ESC to exit.\n");
    
    while (1) {
        char c = keyboard_get_char();
        
        if (c == 27) break; // ESC to exit
        
        if (c == '\n') {
            es->cy++;
            es->cx = 0;
        } else if (c == '\b') {
            if (es->cx > 0) {
                es->cx--;
                es->lines[es->cy][es->cx] = '\0';
            }
        } else if (c >= 32 && c <= 126) {
            if (es->cx < MAX_COLS - 1) {
                es->lines[es->cy][es->cx++] = c;
            }
        }
        
        // Re-draw is expensive, in a real OS we'd only update the changed part
        editor_draw(es);
    }
    vga_clear();
}
