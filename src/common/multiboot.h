#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_HEADER_MAGIC     0x1BADB002
#define MULTIBOOT_HEADER_FLAGS     0x00000003
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
};

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    // ... more fields if needed
};

struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
};

#endif
