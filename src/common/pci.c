#include "pci.h"
#include "io.h"
#include "stdio.h"

uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
 
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    outb(0xCF8, address);
    return inw(0xCFC + (offset & 2));
}

void pci_init() {
    kprintf("[PCI] Scanning hardware bus...\n");
    for(int bus = 0; bus < 256; bus++) {
        for(int slot = 0; slot < 32; slot++) {
            uint32_t vendor = pci_read_config(bus, slot, 0, 0);
            if(vendor != 0xFFFF && vendor != 0x0000) {
                uint32_t class_info = pci_read_config(bus, slot, 0, 8);
                uint8_t class_code = (class_info >> 24) & 0xFF;
                uint8_t subclass = (class_info >> 16) & 0xFF;
                
                kprintf("Found Device: %x:%x [Class %x:%x]\n", 
                        (uint64_t)vendor, (uint64_t)pci_read_config(bus, slot, 0, 2),
                        (uint64_t)class_code, (uint64_t)subclass);
            }
        }
    }
}
