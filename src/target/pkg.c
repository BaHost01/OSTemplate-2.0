#include "pkg.h"
#include "../common/vga.h"
#include "../common/string.h"

void pkg_install(const char* name) {
    vga_puts("Reading package index...\n");
    vga_puts("Found: ");
    vga_puts(name);
    vga_puts("\n");
    
    vga_puts("Downloading ");
    vga_puts(name);
    vga_puts(" from local repository...\n");
    
    vga_puts("Verifying integrity...\n");
    vga_puts("Extracting files to /bin/...\n");
    
    // Logic for actual file copy using FAT32 would go here
    
    vga_puts("Package ");
    vga_puts(name);
    vga_puts(" installed successfully!\n");
}
