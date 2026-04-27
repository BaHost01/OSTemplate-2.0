#include "path.h"
#include "../common/string.h"
#include "../common/vga.h"

static char path_buffer[512];
static char* directories[16];
static int dir_count = 0;

void path_init() {
    memset(path_buffer, 0, sizeof(path_buffer));
    dir_count = 0;
    
    // Default paths
    path_add_directory("C:\\Windows\\System32");
    path_add_directory("C:\\bin");
    path_add_directory("/bin");
}

void path_add_directory(const char* dir) {
    if (dir_count < 16) {
        directories[dir_count++] = (char*)dir;
    }
}

const char* path_resolve_command(const char* cmd) {
    // In a real OS, we would check the disk for each directory in the path
    vga_puts("[PATH] Resolving: ");
    vga_puts(cmd);
    vga_puts("\n");
    return cmd; // For now just return the command
}
