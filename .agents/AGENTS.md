# SimpleOS x64 / OSTemplate-2.0 Workspace Rules

## Early Boot and Paging Constraints
* **Memory Paging Capacity**: When setting up the early identity paging (PML4 -> PDPT -> PD) in `boot.s`, you MUST identity-map at least the first 512MB to 1GB of physical memory using 2MB huge pages. Do NOT map only 2MB, as the Multiboot2 information structure (MBI) or the Framebuffer (video memory) can be placed at high physical addresses by the bootloader (e.g., above 2MB or in high PCI memory space), triggering an immediate Page Fault (#PF) during kernel initialization.
* **Multiboot2 Tag Alignment**: All tags defined in the assembly headers (`boot.s`) or parsed at runtime in C (`kernel.c`) must be 8-byte aligned. When iterating tags in C, always use `(tag->size + 7) & ~7` to calculate the offset to the next tag.

## Video and Graphics Configuration
* **GRUB Video Control**: If requesting a graphical mode (framebuffer), you must ensure `grub.cfg` includes:
  ```grub
  insmod all_video
  set gfxmode=1024x768x32
  set gfxpayload=keep
  ```
  The Multiboot2 header in `boot.s` must also contain a matching Framebuffer request tag (Type 5).
