#include "paging.h"
#include "vga.h"
#include "string.h"

// PML4, PDP, PD, PT for 4-level paging in x86_64
uint64_t pml4[512] __attribute__((aligned(4096)));
uint64_t pdp[512] __attribute__((aligned(4096)));
uint64_t pd[512] __attribute__((aligned(4096)));

void paging_init() {
    memset(pml4, 0, 4096);
    memset(pdp, 0, 4096);
    memset(pd, 0, 4096);

    // Identity map the first 512MB using 2MB huge pages
    // We add the User bit (0x04) so Ring 3 applications can access memory.
    // 0x87 = Present (1) | Write (2) | User (4) | Huge (128)
    for(int i = 0; i < 256; i++) {
        pd[i] = (i * 0x200000) | 0x87; 
    }

    // 0x07 = Present (1) | Write (2) | User (4)
    pdp[0] = ((uint64_t)pd) | 0x07;
    pml4[0] = ((uint64_t)pdp) | 0x07;

    // Load PML4 into CR3
    asm volatile("mov %0, %%cr3" :: "r"(pml4));

    vga_puts("x64 4-Level Paging Initialized (512MB Identity, User Accessible)\n");
}
