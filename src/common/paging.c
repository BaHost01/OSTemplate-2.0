#include "paging.h"
#include "vga.h"
#include "string.h"

// PML4, PDP, PD, PT for 4-level paging in x86_64
uint64_t pml4[512] __attribute__((aligned(4096)));
uint64_t pdp[512] __attribute__((aligned(4096)));
uint64_t pd[512] __attribute__((aligned(4096)));
uint64_t pt[512] __attribute__((aligned(4096)));

void paging_init() {
    memset(pml4, 0, 4096);
    memset(pdp, 0, 4096);
    memset(pd, 0, 4096);
    memset(pt, 0, 4096);

    // Identity map the first 2MB using 4KB pages
    for(int i = 0; i < 512; i++) {
        pt[i] = (i * 0x1000) | 3; // Present, Write
    }

    pd[0] = ((uint64_t)pt) | 3;
    pdp[0] = ((uint64_t)pd) | 3;
    pml4[0] = ((uint64_t)pdp) | 3;

    // Load PML4 into CR3
    asm volatile("mov %0, %%cr3" :: "r"(pml4));

    vga_puts("x64 4-Level Paging Initialized\n");
}
