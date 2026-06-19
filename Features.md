# Features

## Complete Production Rewrite

- [x] True Ring 3 Userland Environment
- [x] Standard C Library (`libc`) with Syscall Wrappers
- [x] ELF Binary Loader (`elf.c`)
- [x] Initial Ramdisk (`initrd.tar`) Support
- [x] Process Management / Task Subsystem
- [x] Base Core Utilities (`init`, `sh`, `ls`, `cat`)

## Legacy & Existing Features

- Multiboot 2 Bootloader Compatibility
- Graphical Framebuffer Support (Linear)
- x86_64 Long Mode with 4-level Paging Identity Mapping
- Dual Subsystem API (Linux Syscalls via `0x80`, Win32/NT via `0x2E`)
- Minimal PCI & ATA Stub Drivers
- Object Manager for kernel handles
