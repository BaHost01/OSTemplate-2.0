#include "../../libc/stdio.h"
#include "../../libc/syscall.h"

int main(int argc, char *argv[]) {
    // For now, since we don't pass argc/argv cleanly from the shell yet,
    // we'll just print a stub message, or read a hardcoded file.
    (void)argc;
    (void)argv;
    
    printf("cat: Not fully implemented yet. Need argc/argv parsing.\n");
    return 0;
}
