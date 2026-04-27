#include "../common/multiboot.h"
#include "../common/vga.h"
#include "../common/gdt.h"
#include "../common/idt.h"
#include "../common/paging.h"
#include "../common/pci.h"
#include "../common/keyboard.h"
#include "../common/string.h"
#include "../common/kheap.h"
#include "../common/stdio.h"
#include "../common/serial.h"
#include "../common/vfs.h"
#include "edit.h"
#include "pkg.h"
#include "subsystem.h"
#include "iomgr.h"
#include "desktop.h"
#include "path.h"

/*
 * shell - Primary interactive command processor.
 * Operates in a continuous loop, parsing input and dispatching to 
 * internal kernel functions or future Ring 3 processes.
 */
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
                    vga_puts("SimpleOS x64 Ultimate v1.0 (Clean-Room NT Architecture)\n");
                } else if (strcmp(line, "edit") == 0) {
                    editor_main();
                } else if (strncmp(line, "pkg install ", 12) == 0) {
                    pkg_install(line + 12);
                } else {
                    kprintf("Command not found: %s\n", line);
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

/*
 * kmain - The high-level entry point for the x86_64 Long Mode Kernel.
 * Initializes core hardware subsystems, memory management, and executive managers
 * before handing control to the user interface.
 */
void kmain(uint32_t magic, struct multiboot_info* info) {
    (void)magic; (void)info;

    /* Initialize critical display and communication interfaces */
    vga_init();
    serial_init();
    
    /* Setup Global Descriptor Table and Interrupt Descriptor Table */
    gdt_init();
    idt_init();
    
    /* Initialize memory management: Physical PMM, Paging, and Executive Heap */
    paging_init();
    kheap_init();
    
    /* Initialize Virtual File System and Hardware Bus Management */
    vfs_init();
    pci_init();
    
    /* Initialize NT-Compatible Executive Subsystems */
    IoInit();
    path_init();
    subsystem_init();
    
    serial_puts("[Kernel] SimpleOS x64 Executive Boot Sequence Complete\n");
    
    vga_set_color(0x0E, 0x00); // Gold branding
    vga_puts("--- SimpleOS x64 Ultimate - High Performance Kernel ---\n");
    vga_puts("Architecture: x86_64 Long Mode | Boot: GPT/UEFI Ready\n");
    vga_puts("System initialized. Type 'desktop' for UI or 'help' for commands.\n");
    
    shell();
}
