#include "initrd.h"
#include "string.h"
#include "stdio.h"

static uint64_t initrd_start = 0;
static uint32_t initrd_size = 0;

typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} __attribute__((packed)) tar_header_t;

static uint32_t parse_octal(const char *str, size_t size) {
    uint32_t value = 0;
    for (size_t i = 0; i < size && str[i] != 0 && str[i] != ' '; i++) {
        if (str[i] >= '0' && str[i] <= '7') {
            value = value * 8 + (str[i] - '0');
        }
    }
    return value;
}

void initrd_init(uint64_t addr, uint32_t size) {
    initrd_start = addr;
    initrd_size = size;
    kprintf("[INITRD] Mounted ramdisk at %p, size: %d bytes\n", (void*)addr, size);
}

const uint8_t* initrd_get_file(const char* filename, size_t* out_size) {
    if (!initrd_start) return NULL;
    
    uint64_t current = initrd_start;
    while (current < initrd_start + initrd_size) {
        tar_header_t *header = (tar_header_t *)current;
        if (header->name[0] == '\0') {
            break; // End of archive
        }
        
        uint32_t file_size = parse_octal(header->size, 11);
        
        // Handle name (sometimes prefix + name)
        // A real TAR parser would handle 'prefix', but we keep it simple
        if (strcmp(header->name, filename) == 0) {
            if (out_size) *out_size = file_size;
            return (const uint8_t *)(current + 512);
        }
        
        current += 512 + ((file_size + 511) & ~511);
    }
    
    return NULL;
}
