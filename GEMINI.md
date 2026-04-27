# GEMINI.md - OSTemplate-2.0

## Project Overview
**OSTemplate-2.0** is a template repository designed for Operating System development. It provides a foundation for building a simple x86-based OS with a focus on automation through CI/CD.

The project is structured to support:
- **Kernel Development:** Using Assembly (NASM) and likely C.
- **Bootloading:** Utilizing GRUB and the Multiboot standard.
- **Automated Builds:** A GitHub Actions workflow is provided to compile the kernel and generate a bootable ISO.

## Project Structure (Inferred)
Based on the CI configuration, the project expects the following structure:
- `Makefile`: To orchestrate the build process.
- Source files (Assembly/C): To be compiled into `kernel.bin`.
- `build/`: Target directory for compiled artifacts.
- `iso/`: Temporary directory for ISO structure.

*Note: Current repository appears to be a skeleton; core source files and the Makefile are currently missing.*

## Building and Running
The build process is defined in `.github/workflows/main.yml`.

### Prerequisites
To build the project locally, you will need:
- `build-essential` (make, gcc, etc.)
- `nasm`
- `xorriso`
- `grub-pc-bin`
- `grub-common`

### Commands
- **Build Kernel:**
  ```bash
  make all
  ```
- **Create ISO:**
  The ISO is created via `grub-mkrescue`. Refer to the GitHub workflow for the exact steps to generate `myos.iso`.

### TODO
- [ ] Create a `Makefile` to handle compilation.
- [ ] Add initial kernel entry point (e.g., `boot.s`).
- [ ] Implement basic kernel functionality.

## Development Conventions
- **CI/CD:** Automated builds are triggered on every push and pull request via GitHub Actions.
- **Target:** x86 architecture with Multiboot compliance.
- **License:** MIT License.
