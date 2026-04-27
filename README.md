# MyOS x64 Ultimate

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Arch](https://img.shields.io/badge/arch-x86__64-orange.svg)
![Status](https://img.shields.io/badge/status-semi--production-green.svg)

**MyOS x64 Ultimate** is a high-performance, hybrid kernel operating system designed with a "Clean-Room" NT-inspired architecture. It supports both **Linux (ELF)** and **Windows (PE)** binary formats, featuring a robust 64-bit executive.

## 🚀 Key Features

- **Hybrid Executive:** Native support for Windows (Win32/NT) and Linux (POSIX) syscalls.
- **x64 Long Mode:** Full 64-bit architecture with 4-level paging (PML4).
- **GPT Support:** Native GUID Partition Table support for modern storage.
- **WDM-Style Drivers:** Layered driver model for USB, Network (WiFi/Ethernet), and Bluetooth.
- **Object Manager:** Handle-based resource management (inspired by NT).
- **Customizable UI:** Modular terminal-based interface with future support for Lucide-icon based graphics.

## 📁 Project Structure

- `src/common/`: Shared kernel components (GDT, IDT, Paging, Memory).
- `src/installer/`: Bare-metal OS installer with GPT/FAT32 support.
- `src/target/`: The main hybrid kernel (The "Hybrid Target").
- `Third_Party/`: Reference architecture and assets (Lucide icons).

## 🛠️ Build Requirements

- `clang` / `lld` (LLVM Toolchain)
- `nasm` (Assembler)
- `xorriso` / `grub-mkrescue` (ISO generation)
- `mtools`

```bash
make all
```

## 🔒 Security

Please refer to [SECURITY.md](SECURITY.md) for vulnerability reporting and security policies.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
