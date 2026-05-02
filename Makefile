CC = clang
LD = ld.lld
AS = nasm

CFLAGS = -target x86_64-unknown-none-elf -march=x86-64 -ffreestanding -O3 -Wall -Wextra -fno-stack-protector -fno-builtin -nostdlib -mno-red-zone -Isrc/common
ASFLAGS = -f elf64
LDFLAGS = -m elf_x86_64 -T src/common/linker.ld --gc-sections

COMMON_OBJS = build/common/vga.o build/common/gdt.o build/common/gdt_flush.o \
              build/common/idt.o build/common/interrupts.o build/common/isr_handler.o \
              build/common/pmm.o build/common/paging.o build/common/vfs.o build/common/ata.o build/common/pci.o \
              build/common/serial.o build/common/stdio.o build/common/string.o build/common/keyboard.o build/common/kheap.o
INSTALLER_OBJS = build/installer/boot.o build/installer/kernel.o $(COMMON_OBJS)
TARGET_OBJS = build/target/boot.o build/target/kernel.o build/target/edit.o build/target/pkg.o \
              build/target/subsystem.o build/target/object_manager.o build/target/ke.o \
              build/target/pe_loader.o build/target/iomgr.o build/target/wifi.o \
              build/target/desktop.o build/target/ksod.o build/target/path.o \
              build/target/ex.o $(COMMON_OBJS)

all: build/installer.bin build/target.bin iso/myos.iso

build/common/%.o: src/common/%.c
	mkdir -p build/common
	$(CC) $(CFLAGS) -c $< -o $@

build/common/%.o: src/common/%.s
	mkdir -p build/common
	$(AS) $(ASFLAGS) $< -o $@

build/installer/%.o: src/installer/%.c
	mkdir -p build/installer
	$(CC) $(CFLAGS) -c $< -o $@

build/installer/%.o: src/installer/%.s
	mkdir -p build/installer
	$(AS) $(ASFLAGS) $< -o $@

build/target/%.o: src/target/%.c
	mkdir -p build/target
	$(CC) $(CFLAGS) -c $< -o $@

build/target/pe_loader.o: src/target/pe_loader.c
	mkdir -p build/target
	$(CC) $(CFLAGS) -c $< -o $@

build/target/%.o: src/target/%.s
	mkdir -p build/target
	$(AS) $(ASFLAGS) $< -o $@

build/installer.bin: $(INSTALLER_OBJS)
	$(LD) $(LDFLAGS) $(INSTALLER_OBJS) -o $@

build/target.bin: $(TARGET_OBJS)
	$(LD) $(LDFLAGS) $(TARGET_OBJS) -o $@

iso/myos.iso: build/installer.bin build/target.bin
	mkdir -p iso/boot/grub
	cp build/installer.bin iso/boot/installer.bin
	cp build/target.bin iso/boot/target.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'set gfxpayload=text' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS Installer" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot2 /boot/installer.bin' >> iso/boot/grub/grub.cfg
	echo '  module2 /boot/target.bin "target_kernel"' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	# Since we don't have grub-mkrescue, we'll try to use xorriso directly
	# This requires a bootloader. Since we can't easily get grub-pc-bin,
	# maybe we can just build the kernels for now and verify they compile.
	# For actual ISO creation, we'd need a bootloader.
	# I'll add a placeholder or a comment.
	@echo "ISO creation requires grub-mkrescue which is not available."
	@echo "Kernels built successfully at build/installer.bin and build/target.bin"

clean:
	rm -rf build iso/boot/*.bin
