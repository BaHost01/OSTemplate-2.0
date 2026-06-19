# SimpleOS x64

[![SimpleOS x64 Pipeline](https://github.com/BaHost01/OSTemplate-2.0/actions/workflows/main.yml/badge.svg)](https://github.com/BaHost01/OSTemplate-2.0/actions/workflows/main.yml)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Arch](https://img.shields.io/badge/arch-x86__64-orange.svg)
![Status](https://img.shields.io/badge/status-production-green.svg)

**SimpleOS x64** is a high-performance, hybrid kernel operating system designed with a "Clean-Room" NT-inspired architecture. It features a complete transition from a monolithic Ring 0 design to a full production architecture with a robust **Ring 3 Userland**, standard C library (`libc`), and an ELF loader.

## 🚀 Key Features

- **True Ring 3 Userland:** Applications run in isolated Ring 3 environments via 4-level paging and TSS.
- **Minimal Libc:** Includes a standard C library bridge for `syscalls`, `stdio`, `string`, and `malloc`.
- **Hybrid Executive:** Native support for Windows (Win32/NT) and Linux (POSIX) syscalls.
- **Initramfs (Initrd):** Loads an initial ramdisk containing essential userland applications (`init`, `sh`, `ls`, `cat`).
- **x64 Long Mode:** Full 64-bit architecture with 4-level paging (PML4).
- **GPT Support:** Native GUID Partition Table support for modern storage.
- **WDM-Style Drivers:** Layered driver model for USB, Network (WiFi/Ethernet), and Bluetooth.
- **Object Manager:** Handle-based resource management (inspired by NT).

## 📁 Project Structure

- `src/common/`: Shared kernel components (GDT, IDT, Paging, Memory, ELF, Initrd, Tasking).
- `src/installer/`: Bare-metal OS installer with GPT/FAT32 support.
- `src/target/`: The main hybrid kernel (The "Hybrid Target").
- `src/libc/`: Standard C Library for Userland.
- `src/apps/`: Userland applications (`init`, `sh`, `coreutils`).

## 🛠️ Build Requirements

- `clang` / `lld` (LLVM Toolchain)
- `nasm` (Assembler)
- `llvm-ar` (Archiver for libc)
- `xorriso` / `grub-mkrescue` (ISO generation)
- `mtools`

```bash
make all
```

## 🔒 Security

Please refer to [SECURITY.md](SECURITY.md) for vulnerability reporting and security policies.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
