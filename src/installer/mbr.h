#ifndef MBR_H
#define MBR_H

#include <stdint.h>

struct mbr_partition {
    uint8_t bootable;
    uint8_t start_head;
    uint8_t start_sector;
    uint8_t start_cylinder;
    uint8_t sys_id;
    uint8_t end_head;
    uint8_t end_sector;
    uint8_t end_cylinder;
    uint32_t start_lba;
    uint32_t total_sectors;
} __attribute__((packed));

struct mbr {
    uint8_t bootstrap[446];
    struct mbr_partition partitions[4];
    uint16_t signature;
} __attribute__((packed));

#endif
