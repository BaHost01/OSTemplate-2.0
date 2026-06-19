#include "../../libc/stdio.h"
#include "../../libc/string.h"
#include "../../libc/syscall.h"

#define MAX_LINE 128

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    char line[MAX_LINE];
    char buf[1];
    
    printf("\nWelcome to SimpleOS Shell!\n");
    
    while (1) {
        printf("$ ");
        
        int i = 0;
        while (i < MAX_LINE - 1) {
            long ret = sys_read(0, buf, 1);
            if (ret <= 0) continue;
            
            char c = buf[0];
            if (c == '\n' || c == '\r') {
                printf("\n");
                break;
            } else if (c == '\b' || c == 127) { // Backspace
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else {
                line[i++] = c;
                putchar(c);
            }
        }
        line[i] = '\0';
        
        if (i > 0) {
            if (strcmp(line, "help") == 0) {
                printf("Built-in commands:\n");
                printf("  help   - Show this message\n");
                printf("  ls     - List files\n");
                printf("  cat    - Read a file\n");
            } else {
                // Try to execute the command
                char *cmd_argv[] = {line, NULL};
                char *cmd_envp[] = {NULL};
                
                // Prepend /bin/ if not absolute path
                char path[MAX_LINE];
                if (line[0] != '/') {
                    strcpy(path, "/bin/");
                    strcpy(path + 5, line);
                } else {
                    strcpy(path, line);
                }
                
                long ret = sys_execve(path, cmd_argv, cmd_envp);
                if (ret < 0) {
                    printf("Command not found: %s\n", line);
                }
            }
        }
    }
    
    return 0;
}
