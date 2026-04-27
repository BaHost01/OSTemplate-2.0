#include "../common/pe.h"
#include "../common/paging.h"
#include "../common/stdio.h"
#include "../common/string.h"
#include "../common/kheap.h"

int load_pe_executable(void* file_data) {
    mz_header* mz = (mz_header*)file_data;
    if (mz->magic != MZ_MAGIC) return -1;

    pe_header* pe = (pe_header*)((uint8_t*)file_data + mz->pe_offset);
    if (pe->signature != PE_MAGIC) return -1;

    kprintf("[PE Loader] Loading Windows Executable (Sections: %d)\n", (uint64_t)pe->num_sections);
    
    pe_section_header* section = (pe_section_header*)((uint8_t*)pe + sizeof(pe_header) + pe->opt_header_size);

    for (int i = 0; i < pe->num_sections; i++) {
        char name[9];
        memcpy(name, section[i].name, 8);
        name[8] = '\0';
        kprintf("  Section: %s @ %p (Size: %x)\n", name, (uint64_t)section[i].virtual_address, (uint64_t)section[i].virtual_size);
    }

    kprintf("[DLL] Checking dependencies...\n");
    return 0;
}

int load_dll(const char* dll_name) {
    kprintf("[DLL] Mapping library: %s\n", dll_name);
    return 0;
}
