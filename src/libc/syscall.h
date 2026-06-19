#ifndef LIBC_SYSCALL_H
#define LIBC_SYSCALL_H

#include <stdint.h>
#include <stddef.h>

// Linux syscall numbers
#define SYS_READ    0
#define SYS_WRITE   1
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_EXECVE  59
#define SYS_EXIT    60

long syscall0(long n);
long syscall1(long n, long a1);
long syscall2(long n, long a1, long a2);
long syscall3(long n, long a1, long a2, long a3);
long syscall4(long n, long a1, long a2, long a3, long a4);
long syscall5(long n, long a1, long a2, long a3, long a4, long a5);

long sys_read(int fd, void *buf, size_t count);
long sys_write(int fd, const void *buf, size_t count);
long sys_open(const char *filename, int flags, int mode);
long sys_close(int fd);
long sys_execve(const char *filename, char *const argv[], char *const envp[]);
void sys_exit(int status);

#endif
