#include "../common/multiboot.h"
#include "../common/vga.h"
#include "../common/gdt.h"
#include "../common/idt.h"
#include "../common/paging.h"
#include "../common/pci.h"
#include "../common/keyboard.h"
#include "../common/string.h"
#include "../common/kheap.h"
#include "edit.h"
#include "pkg.h"
#include "subsystem.h"
#include "iomgr.h"
#include "desktop.h"
#include "path.h"
#include "../common/vfs.h"
#include "../common/serial.h"

void shell() {
    char line[80];
    int i = 0;

    vga_puts("> ");
    while (1) {
        char c = keyboard_get_char();
        if (c == '\n') {
            line[i] = '\0';
            vga_putc('\n');
            
            if (strlen(line) > 0) {
                if (strcmp(line, "help") == 0) {
                    vga_puts("Available commands: help, cls, version, edit, desktop, pkg install <name>\n");
                } else if (strcmp(line, "cls") == 0) {
                    vga_clear();
                } else if (strcmp(line, "desktop") == 0) {
                    desktop_show();
                } else if (strcmp(line, "version") == 0) {
                    vga_puts("MyOS x64 Ultimate v1.0 (Clean-Room NT)\n");
                } else if (strcmp(line, "edit") == 0) {
                    editor_main();
                } else if (strncmp(line, "pkg install ", 12) == 0) {
                    pkg_install(line + 12);
                } else {
                    vga_puts("Command not found: ");
                    vga_puts(line);
                    vga_putc('\n');
                }
            }
            
            i = 0;
            vga_puts("> ");
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                vga_putc('\b');
            }
        } else if (c >= 32 && c <= 126) {
            if (i < 79) {
                line[i++] = c;
                vga_putc(c);
            }
        }
    }
}

void kmain(uint32_t magic, struct multiboot_info* info) {
    vga_init();
    serial_init();
    gdt_init();
    idt_init();
    paging_init();
    vfs_init();
    pci_init();
    IoInit();
    path_init();
    subsystem_init();
    
    serial_puts("[Kernel] Serial Logging Active (COM1)\n");
    serial_puts("[Kernel] x64 Long Mode Initialized\n");
    
    // Welcome to the high-performance x64 environment
    vga_set_color(0x0E, 0x00); // Gold
    vga_puts("--- MyOS x64 Ultimate - High Performance Kernel ---\n");
    vga_puts("Architecture: x86_64 Long Mode | Boot: GPT/UEFI Ready\n");
    vga_puts("System initialized. Type 'desktop' for UI or 'help' for commands.\n");
    
    shell();
}
