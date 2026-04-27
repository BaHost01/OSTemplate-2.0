#include "subsystem.h"
#include "object_manager.h"
#include "../common/vga.h"
#include "../common/idt.h"
#include "../common/stdio.h"

void handle_linux_syscall(struct registers* r) {
    uint64_t syscall_no = r->rax;
    uint64_t arg1 = r->rdi;
    uint64_t arg2 = r->rsi;
    
    switch(syscall_no) {
        case 1: // sys_write
            // arg1 = fd, arg2 = buf
            kprintf("[Syscall] write(fd: %d, buf: %s)\n", arg1, (char*)arg2);
            break;
        case 60: // sys_exit
            kprintf("[Syscall] exit(%d)\n", arg1);
            // task_exit(arg1);
            break;
        default:
            kprintf("[Syscall] Unknown Linux Syscall: %d\n", syscall_no);
            break;
    }
}

void handle_win32_syscall(struct registers* r) {
    uint64_t call_no = r->rax;
    kprintf("[NT] Service Call: %x\n", call_no);
    
    // Simulate NtDisplayString
    if (call_no == 0x01) {
        kprintf("%s", (char*)r->rdx);
    }
}

void subsystem_init() {
    ob_init();
    
    // Register Linux Syscall Interrupt
    idt_set_gate(0x80, (uint64_t)handle_linux_syscall, 0x08, 0xEE);
    
    // Register Windows Syscall Interrupt (Legacy NT)
    idt_set_gate(0x2E, (uint64_t)handle_win32_syscall, 0x08, 0xEE);
    
    vga_puts("NT and POSIX System Call Interfaces Ready\n");
}
