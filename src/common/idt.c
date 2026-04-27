#include "idt.h"
#include "string.h"

static struct idt_entry idt[256];
static struct idt_ptr ip;

extern void idt_load(uint64_t);

extern void isr0();
extern void isr1();

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_mid = (base >> 16) & 0xFFFF;
    idt[num].base_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].sel = sel;
    idt[num].ist = 0;
    idt[num].flags = flags;
    idt[num].reserved = 0;
}

void idt_init() {
    ip.limit = sizeof(struct idt_entry) * 256 - 1;
    ip.base = (uint64_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);

    idt_load((uint64_t)&ip);
}
