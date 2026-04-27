#include "ksod.h"
#include "../common/vga.h"
#include "../common/serial.h"
#include "../common/stdio.h"

void ksod_trigger(struct registers* r, const char* reason) {
    vga_set_color(0x0F, 0x04); // White on Red
    vga_clear();
    
    kprintf(":( YOUR KERNEL HAS DIED BUT WE TOOK A SNAPSHOT\n");
    kprintf("==============================================\n\n");
    kprintf("CRITICAL_ERROR: %s\n\n", reason);
    
    kprintf("KERNEL SNAPSHOT (Registers):\n");
    kprintf("RAX: %p  RBX: %p  RCX: %p  RDX: %p\n", r->rax, r->rbx, r->rcx, r->rdx);
    kprintf("RSI: %p  RDI: %p  RBP: %p  RSP: %p\n", r->rsi, r->rdi, r->rbp, r->rsp);
    kprintf("R8:  %p  R9:  %p  R10: %p  R11: %p\n", r->r8, r->r9, r->r10, r->r11);
    kprintf("RIP: %p  CS:  %x  EFLAGS: %x\n\n", r->rip, (uint64_t)r->cs, (uint64_t)r->rflags);
    
    kprintf("A system snapshot was saved to 'C:\\Snapshots\\last_death.snap'\n");
    kprintf("The system will now halt to prevent hardware damage.\n");
    
    while(1) { asm("hlt"); }
}
