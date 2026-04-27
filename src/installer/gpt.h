#ifndef GPT_H
#define GPT_H

#include <stdint.h>

typedef struct {
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t  data4[8];
} guid_t;

typedef struct {
    char     signature[8]; // "EFI PART"
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t reserved;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    guid_t   disk_guid;
    uint64_t partition_entry_lba;
    uint32_t num_partition_entries;
    uint32_t partition_entry_size;
    uint32_t partition_entry_crc32;
} __attribute__((packed)) gpt_header;

typedef struct {
    guid_t   partition_type_guid;
    guid_t   unique_partition_guid;
    uint64_t starting_lba;
    uint64_t ending_lba;
    uint64_t attributes;
    uint16_t name[36]; // UTF-16
} __attribute__((packed)) gpt_entry;

#endif
