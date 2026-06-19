#include "../../libc/stdio.h"
#include "../../libc/syscall.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    printf("\n");
    printf("==========================================\n");
    printf("=   SimpleOS x64 - Userland INIT (PID 1) =\n");
    printf("==========================================\n");
    
    // Attempt to execute the shell
    char *sh_argv[] = {"/bin/sh", NULL};
    char *sh_envp[] = {NULL};
    
    printf("[INIT] Starting /bin/sh...\n");
    
    long ret = sys_execve("/bin/sh", sh_argv, sh_envp);
    
    // If execve returns, it failed
    printf("[INIT] Failed to start /bin/sh. Error: %d\n", (int)ret);
    
    while(1) {
        // Sleep or wait for children in a real init
        // For now, just spin
    }
    
    return 0;
}
