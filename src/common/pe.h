#ifndef PE_H
#define PE_H

#include <stdint.h>

#define MZ_MAGIC 0x5A4D
#define PE_MAGIC 0x00004550

typedef struct {
    uint16_t magic;         // MZ Magic
    uint8_t  ignored[58];
    uint32_t pe_offset;     // Offset to PE Header
} __attribute__((packed)) mz_header;

typedef struct {
    uint32_t signature;     // PE\0\0
    uint16_t machine;
    uint16_t num_sections;
    uint32_t timestamp;
    uint32_t sym_table;
    uint32_t num_syms;
    uint16_t opt_header_size;
    uint16_t characteristics;
} __attribute__((packed)) pe_header;

typedef struct {
    char     name[8];
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_linenumbers;
    uint16_t num_relocations;
    uint16_t num_linenumbers;
    uint32_t characteristics;
} __attribute__((packed)) pe_section_header;

typedef struct {
    uint32_t original_first_thunk;
    uint32_t time_date_stamp;
    uint32_t forwarder_chain;
    uint32_t name;
    uint32_t first_thunk;
} __attribute__((packed)) pe_import_descriptor;

#endif
