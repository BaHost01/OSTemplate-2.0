CC = clang
LD = ld.lld
AS = nasm
AR = llvm-ar

CFLAGS = -target x86_64-unknown-none-elf -march=x86-64 -ffreestanding -O3 -Wall -Wextra -fno-stack-protector -fno-builtin -nostdlib -mno-red-zone -Isrc/common
ASFLAGS = -f elf64
LDFLAGS = -m elf_x86_64 -T src/common/linker.ld --gc-sections

COMMON_OBJS = build/common/vga.o build/common/font.o build/common/gdt.o build/common/gdt_flush.o \
              build/common/idt.o build/common/interrupts.o build/common/isr_handler.o \
              build/common/pmm.o build/common/paging.o build/common/vfs.o build/common/ata.o build/common/pci.o \
              build/common/serial.o build/common/stdio.o build/common/string.o build/common/keyboard.o build/common/kheap.o \
              build/common/elf.o build/common/initrd.o build/common/task.o build/common/fat32.o

INSTALLER_OBJS = build/installer/boot.o build/installer/kernel.o $(COMMON_OBJS)
TARGET_OBJS = build/target/boot.o build/target/kernel.o build/target/edit.o build/target/pkg.o \
              build/target/subsystem.o build/target/object_manager.o build/target/ke.o \
              build/target/pe_loader.o build/target/iomgr.o build/target/wifi.o \
              build/target/desktop.o build/target/ksod.o build/target/path.o \
              build/target/ex.o $(COMMON_OBJS)

LIBC_SRCS = src/libc/syscall.c src/libc/stdio.c src/libc/string.c src/libc/malloc.c
LIBC_OBJS = $(patsubst src/libc/%.c, build/libc/%.o, $(LIBC_SRCS)) build/libc/crt0.o
APP_LDFLAGS = -m elf_x86_64 -T src/libc/user_linker.ld --gc-sections

APPS = build/apps/init build/apps/sh build/apps/ls build/apps/cat

all: build/installer.bin build/target.bin build/initrd.tar iso/myos.iso

# Kernel compilation rules
build/common/%.o: src/common/%.c
	@mkdir -p build/common
	$(CC) $(CFLAGS) -c $< -o $@

build/common/%.o: src/common/%.s
	@mkdir -p build/common
	$(AS) $(ASFLAGS) $< -o $@

build/installer/%.o: src/installer/%.c
	@mkdir -p build/installer
	$(CC) $(CFLAGS) -c $< -o $@

build/installer/%.o: src/installer/%.s
	@mkdir -p build/installer
	$(AS) $(ASFLAGS) $< -o $@

build/target/%.o: src/target/%.c
	@mkdir -p build/target
	$(CC) $(CFLAGS) -c $< -o $@

build/target/pe_loader.o: src/target/pe_loader.c
	@mkdir -p build/target
	$(CC) $(CFLAGS) -c $< -o $@

build/target/%.o: src/target/%.s
	@mkdir -p build/target
	$(AS) $(ASFLAGS) $< -o $@

build/installer.bin: $(INSTALLER_OBJS)
	$(LD) $(LDFLAGS) $(INSTALLER_OBJS) -o $@

build/target.bin: $(TARGET_OBJS)
	$(LD) $(LDFLAGS) $(TARGET_OBJS) -o $@

# Libc compilation rules
build/libc/%.o: src/libc/%.c
	@mkdir -p build/libc
	$(CC) $(CFLAGS) -c $< -o $@

build/libc/crt0.o: src/libc/crt0.s
	@mkdir -p build/libc
	$(AS) $(ASFLAGS) $< -o $@

build/libc.a: $(LIBC_OBJS)
	@mkdir -p build
	$(AR) rcs $@ $^

# App compilation rules
build/apps/%.o: src/apps/init/%.c
	@mkdir -p build/apps
	$(CC) $(CFLAGS) -c $< -o $@

build/apps/%.o: src/apps/sh/%.c
	@mkdir -p build/apps
	$(CC) $(CFLAGS) -c $< -o $@

build/apps/%.o: src/apps/coreutils/%.c
	@mkdir -p build/apps
	$(CC) $(CFLAGS) -c $< -o $@

build/apps/init: build/apps/init.o build/libc.a build/libc/crt0.o
	$(LD) $(APP_LDFLAGS) build/libc/crt0.o build/apps/init.o build/libc.a -o $@

build/apps/sh: build/apps/sh.o build/libc.a build/libc/crt0.o
	$(LD) $(APP_LDFLAGS) build/libc/crt0.o build/apps/sh.o build/libc.a -o $@

build/apps/ls: build/apps/ls.o build/libc.a build/libc/crt0.o
	$(LD) $(APP_LDFLAGS) build/libc/crt0.o build/apps/ls.o build/libc.a -o $@

build/apps/cat: build/apps/cat.o build/libc.a build/libc/crt0.o
	$(LD) $(APP_LDFLAGS) build/libc/crt0.o build/apps/cat.o build/libc.a -o $@

# Initrd packaging
build/initrd.tar: $(APPS)
	cd build/apps && tar -cf ../initrd.tar init sh ls cat

# ISO Generation
iso/myos.iso: build/installer.bin build/target.bin build/initrd.tar
	mkdir -p iso/boot/grub
	cp build/installer.bin iso/boot/installer.bin
	cp build/target.bin iso/boot/target.bin
	cp build/initrd.tar iso/boot/initrd.tar
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'insmod all_video' >> iso/boot/grub/grub.cfg
	echo 'set gfxmode=1024x768x32' >> iso/boot/grub/grub.cfg
	echo 'set gfxpayload=keep' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS Installer" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot2 /boot/installer.bin' >> iso/boot/grub/grub.cfg
	echo '  module2 /boot/target.bin "target_kernel"' >> iso/boot/grub/grub.cfg
	echo '  module2 /boot/initrd.tar "initrd"' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	@echo "ISO creation requires grub-mkrescue which is not available."
	@echo "Kernels built successfully at build/installer.bin and build/target.bin"

clean:
	rm -rf build iso/boot/*.bin iso/boot/*.tar
