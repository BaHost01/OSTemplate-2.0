#include "ata.h"
#include "io.h"

static void ata_wait_bsy() {
    while (inb(ATA_PRIMARY_STATUS) & 0x80);
}

static void ata_wait_drq() {
    while (!(inb(ATA_PRIMARY_STATUS) & 0x08));
}

void ata_read_sectors(uint32_t lba, uint8_t count, uint16_t* buffer) {
    ata_wait_bsy();
    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, count);
    outb(ATA_PRIMARY_LBA_LOW, (uint8_t)lba);
    outb(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_COMMAND, 0x20); // Read with retry

    for (int i = 0; i < count; i++) {
        ata_wait_bsy();
        ata_wait_drq();
        for (int j = 0; j < 256; j++) {
            buffer[i * 256 + j] = inw(ATA_PRIMARY_DATA);
        }
    }
}

void ata_write_sectors(uint32_t lba, uint8_t count, uint16_t* buffer) {
    ata_wait_bsy();
    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, count);
    outb(ATA_PRIMARY_LBA_LOW, (uint8_t)lba);
    outb(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_COMMAND, 0x30); // Write with retry

    for (int i = 0; i < count; i++) {
        ata_wait_bsy();
        ata_wait_drq();
        for (int j = 0; j < 256; j++) {
            outw(ATA_PRIMARY_DATA, buffer[i * 256 + j]);
        }
    }
}
