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
#include "../common/initrd.h"
#include "../common/task.h"
#include "edit.h"
#include "pkg.h"
#include "subsystem.h"
#include "iomgr.h"
#include "desktop.h"
#include "path.h"

/*
 * kmain - The high-level entry point for the x86_64 Long Mode Kernel.
 * Initializes core hardware subsystems, memory management, and executive managers
 * before handing control to the user interface.
 */
void kmain(uint32_t magic, void* info) {
    uint64_t initrd_loc = 0;
    uint32_t initrd_size = 0;

    if (magic == 0x36d76289) { // Multiboot 2
        struct multiboot2_tag* tag;
        for (tag = (struct multiboot2_tag*)((uint8_t*)info + 8);
             tag->type != 0;
             tag = (struct multiboot2_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
            if (tag->type == 8) { // Framebuffer tag
                vga_init_fb((struct multiboot2_tag_framebuffer*)tag);
            } else if (tag->type == 3) { // Module tag
                // Cast to module tag
                struct {
                    uint32_t type;
                    uint32_t size;
                    uint32_t mod_start;
                    uint32_t mod_end;
                    char string[];
                } *mod = (void*)tag;
                
                // Assuming the first module is our initrd
                if (initrd_loc == 0) {
                    initrd_loc = mod->mod_start;
                    initrd_size = mod->mod_end - mod->mod_start;
                }
            }
        }
    } else {
        vga_init();
    }

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
    task_init();
    
    serial_puts("[Kernel] SimpleOS x64 Executive Boot Sequence Complete\n");
    
    vga_set_color(0x0E, 0x00); // Gold branding
    vga_puts("--- SimpleOS x64 Ultimate - High Performance Kernel ---\n");
    vga_puts("Architecture: x86_64 Long Mode | Boot: GPT/UEFI Ready\n");
    
    if (initrd_loc) {
        initrd_init(initrd_loc, initrd_size);
        vga_puts("Jumping to userland (/bin/init)...\n");
        task_exec("/bin/init");
    } else {
        vga_puts("CRITICAL ERROR: No initrd module loaded. Cannot start userland.\n");
    }
    
    while(1) {
        asm volatile("hlt");
    }
}
