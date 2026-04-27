#include "gdt.h"
#include "string.h"

// Define a unified GDT to ensure memory continuity
typedef struct {
    struct gdt_entry entries[5];
    struct gdt_tss_entry tss_desc;
} __attribute__((packed)) gdt_table_t;

static gdt_table_t gdt;
static struct gdt_ptr gp;
static struct tss_entry tss;

extern void gdt_flush(uint64_t);

static void gdt_set_gate(int num, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt.entries[num].base_low = (base & 0xFFFF);
    gdt.entries[num].base_middle = (base >> 16) & 0xFF;
    gdt.entries[num].base_high = (base >> 24) & 0xFF;
    gdt.entries[num].limit_low = (limit & 0xFFFF);
    gdt.entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt.entries[num].access = access;
}

void gdt_init() {
    gp.limit = sizeof(gdt_table_t) - 1;
    gp.base = (uint64_t)&gdt;

    // 0x00: Null
    gdt_set_gate(0, 0, 0, 0, 0);
    // 0x08: Kernel Code
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); 
    // 0x10: Kernel Data
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // 0x18: User Code (Ring 3)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xAF);
    // 0x20: User Data (Ring 3)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // Setup TSS
    memset(&tss, 0, sizeof(struct tss_entry));
    
    uint64_t tss_base = (uint64_t)&tss;
    gdt.tss_desc.limit_low = sizeof(struct tss_entry);
    gdt.tss_desc.base_low = tss_base & 0xFFFF;
    gdt.tss_desc.base_mid = (tss_base >> 16) & 0xFF;
    gdt.tss_desc.access = 0x89;
    gdt.tss_desc.granularity = 0;
    gdt.tss_desc.base_high = (tss_base >> 24) & 0xFF;
    gdt.tss_desc.base_upper = (tss_base >> 32) & 0xFFFFFFFF;
    gdt.tss_desc.reserved = 0;

    gdt_flush((uint64_t)&gp);
    
    // Load Task Register (TR)
    asm volatile("mov $0x28, %ax; ltr %ax");
}

// Function to drop to Ring 3 (Properly Escaped)
void jump_to_user(uint64_t entry_point, uint64_t stack_ptr) {
    asm volatile(
        "cli;"
        "mov $0x23, %%ax;"
        "mov %%ax, %%ds;"
        "mov %%ax, %%es;"
        "mov %%ax, %%fs;"
        "mov %%ax, %%gs;"
        "pushq $0x23;" // SS
        "pushq %1;"    // RSP
        "pushfq;"      // RFLAGS
        "popq %%rax;"
        "orq $0x200, %%rax;" // Enable interrupts in user mode
        "pushq %%rax;"
        "pushq $0x1B;" // CS
        "pushq %0;"    // RIP
        "iretq"
        : : "r"(entry_point), "r"(stack_ptr) : "rax", "ax"
    );
}
