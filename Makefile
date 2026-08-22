CC = gcc

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin
ASFLAGS = --32
LDFLAGS = -melf_i386

OBJECTS = build/loader.o build/kernel.o

BUILD_DIR = build
ISO = $(BUILD_DIR)/myos.iso

# VirtualBox VM name — change this if your VM is named differently
VM_NAME = MyOS

.PHONY: all install run run_vm clean

# --------------------------------------------------
# Default target
# --------------------------------------------------

all: $(ISO)

# --------------------------------------------------
# Ensure build directory exists
# --------------------------------------------------

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --------------------------------------------------
# Compile C files
# --------------------------------------------------

build/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Assemble .s files
# --------------------------------------------------

build/%.o: %.s | $(BUILD_DIR)
	as $(ASFLAGS) -o $@ $<

# --------------------------------------------------
# Link kernel
# --------------------------------------------------

build/mykernel.bin: linker.ld $(OBJECTS)
	ld $(LDFLAGS) -T linker.ld -o $@ $(OBJECTS)

# --------------------------------------------------
# Copy kernel into ISO directory
# --------------------------------------------------

install: build/mykernel.bin
	mkdir -p iso/boot
	cp $< iso/boot/mykernel.bin

# --------------------------------------------------
# Create bootable ISO using grub-mkrescue
# (handles grub-mkimage + xorriso internally)
# --------------------------------------------------

$(ISO): install | $(BUILD_DIR)
	grub-mkrescue -o $(ISO) iso

# --------------------------------------------------
# Run using QEMU (runs entirely inside WSL)
# --------------------------------------------------

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# --------------------------------------------------
# Run using VirtualBox
# Builds happen in WSL, but VirtualBox itself runs on
# Windows, so this calls the Windows VBoxManage.exe via
# WSL interop and converts the ISO's WSL path to a
# Windows path automatically.
# --------------------------------------------------

run_vm: $(ISO)
	VBoxManage.exe storageattach "$(VM_NAME)" \
		--storagectl "IDE" \
		--port 1 \
		--device 0 \
		--type dvddrive \
		--medium "$$(wslpath -w $(ISO))"

	VBoxManage.exe startvm "$(VM_NAME)"

# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -rf $(BUILD_DIR)
	rm -rf iso/boot