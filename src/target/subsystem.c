#include "subsystem.h"
#include "object_manager.h"
#include "../common/vga.h"
#include "../common/idt.h"
#include "../common/stdio.h"
#include "../common/task.h"

// Basic Syscall routing

void handle_linux_syscall(struct registers* r) {
    uint64_t syscall_no = r->rax;
    uint64_t arg1 = r->rdi;
    uint64_t arg2 = r->rsi;
    uint64_t arg3 = r->rdx;
    
    switch(syscall_no) {
        case 0: // sys_read
            // Very simple stub: if fd==0, read from keyboard
            if (arg1 == 0) {
                char *buf = (char*)arg2;
                // We should implement a blocking keyboard read here
                // For now, we'll just return 0 to avoid hang
                // Actually, we can poll
                extern char keyboard_get_char();
                char c = keyboard_get_char();
                if (c != 0) {
                    buf[0] = c;
                    r->rax = 1;
                } else {
                    r->rax = 0;
                }
            } else {
                r->rax = 0; // Not implemented
            }
            break;
        case 1: // sys_write
            // arg1 = fd, arg2 = buf, arg3 = count
            if (arg1 == 1 || arg1 == 2) {
                char *s = (char*)arg2;
                for (size_t i = 0; i < arg3; i++) {
                    vga_putc(s[i]);
                }
                r->rax = arg3;
            } else {
                r->rax = -1;
            }
            break;
        case 59: // sys_execve
            // arg1 = filename
            task_exec((const char*)arg1);
            // If task_exec returns, it failed
            r->rax = -1;
            break;
        case 60: // sys_exit
            kprintf("[Kernel] Process exited with status %d\n", (int)arg1);
            // In a real OS we'd schedule the next task.
            // For now, just halt.
            while(1);
            break;
        default:
            kprintf("[Syscall] Unknown Linux Syscall: %d\n", (int)syscall_no);
            r->rax = -1;
            break;
    }
}

void handle_win32_syscall(struct registers* r) {
    uint64_t call_no = r->rax;
    kprintf("[NT] Service Call: %x\n", (unsigned int)call_no);
    
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
