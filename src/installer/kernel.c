#include "../common/multiboot.h"
#include "../common/vga.h"
#include "../common/gdt.h"
#include "../common/idt.h"
#include "../common/ata.h"
#include "../common/string.h"
#include "../common/fat32.h"
#include "../common/stdio.h"
#include "mbr.h"

void install(struct multiboot_info* info) {
    kprintf("Starting installation...\n");

    // 1. Create MBR
    struct mbr disk_mbr;
    memset(&disk_mbr, 0, sizeof(struct mbr));
    disk_mbr.signature = 0xAA55;
    
    // Primary partition: Start LBA 2048, Size 65536 sectors (32MB)
    disk_mbr.partitions[0].bootable = 0x80;
    disk_mbr.partitions[0].sys_id = 0x0C; // FAT32 with LBA
    disk_mbr.partitions[0].start_lba = 2048;
    disk_mbr.partitions[0].total_sectors = 65536;
    
    kprintf("Writing MBR...\n");
    ata_write_sectors(0, 1, (uint16_t*)&disk_mbr);

    // 2. Create FAT32 BPB
    struct fat32_bpb bpb;
    memset(&bpb, 0, sizeof(struct fat32_bpb));
    bpb.jmp[0] = 0xEB; bpb.jmp[1] = 0x3C; bpb.jmp[2] = 0x90;
    memcpy(bpb.oem, "SIMPLEOS", 8);
    bpb.bytes_per_sector = 512;
    bpb.sectors_per_cluster = 8;
    bpb.reserved_sectors = 32;
    bpb.num_fats = 2;
    bpb.media_type = 0xF8;
    bpb.total_sectors_32 = 65536;
    bpb.sectors_per_fat_32 = 512;
    bpb.root_cluster = 2;
    bpb.boot_sig = 0x29;
    bpb.volume_id = 0x12345678;
    memcpy(bpb.volume_label, "TARGET OS  ", 11);
    memcpy(bpb.fs_type, "FAT32   ", 8);
    
    kprintf("Formatting FAT32 partition...\n");
    ata_write_sectors(2048, 1, (uint16_t*)&bpb);

    // 3. Copy Target OS Kernel
    if (info->mods_count > 0) {
        struct multiboot_module* mod = (struct multiboot_module*)info->mods_addr;
        uint32_t mod_size = mod->mod_end - mod->mod_start;
        uint32_t sectors_to_write = (mod_size + 511) / 512;
        
        kprintf("Copying Target OS kernel (%d modules found)...\n", (uint64_t)info->mods_count);
        
        // Write kernel starting at LBA 4096 (arbitrary offset inside partition)
        ata_write_sectors(4096, sectors_to_write, (uint16_t*)(uint64_t)mod->mod_start);
    } else {
        kprintf("Error: Target OS module not found!\n");
        return;
    }

    kprintf("Installation complete! Please reboot from the hard drive.\n");
}

void kmain(uint32_t magic, struct multiboot_info* info) {
    vga_init();
    gdt_init();
    idt_init();
    vga_puts("--- OS Installer OS ---\n");
    
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        vga_puts("Error: Invalid magic number\n");
        return;
    }

    install(info);
}
