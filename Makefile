CC = gcc

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin
ASFLAGS = --32
LDFLAGS = -melf_i386

BUILD_DIR = build
ISO = $(BUILD_DIR)/myos.iso

VM_NAME = MyOS

# --------------------------------------------------
# Object files
# --------------------------------------------------
OBJECTS = build/loader.o build/kernel.o build/gdt.o build/port.o build/interruptstub.o build/interrupts.o build/keyboard.o

# --------------------------------------------------
# Targets
# --------------------------------------------------

.PHONY: all install run run_vm clean

all: $(ISO)

# --------------------------------------------------
# Build directory
# --------------------------------------------------

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --------------------------------------------------
# Root-level C files
# --------------------------------------------------

build/kernel.o: kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# C files inside their own directory
#
# gdt/gdt.c -> build/gdt.o
# idt/idt.c -> build/idt.o
# paging/paging.c -> build/paging.o
# --------------------------------------------------

build/gdt.o: gdt/gdt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Port
# --------------------------------------------------

build/port.o: port/port.c port/port.h port/port8.c port/port8Slow.c port/port16.c port/port32.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Keyboard
# --------------------------------------------------

build/keyboard.o:	hardware/keyboard/keyboard.c hardware/keyboard/keyboard.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Interrupts
# --------------------------------------------------

build/interrupts.o: interrupts/interrupts.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

build/interruptstub.o: interrupts/interruptstubs.s | $(BUILD_DIR)
	as $(ASFLAGS) -o $@ $<


# --------------------------------------------------
# Assembly
# --------------------------------------------------

build/loader.o: loader.s | $(BUILD_DIR)
	as $(ASFLAGS) -o $@ $<

# --------------------------------------------------
# Link kernel
# --------------------------------------------------

build/mykernel.bin: linker.ld $(OBJECTS)
	ld $(LDFLAGS) -T linker.ld -o $@ $(OBJECTS)

# --------------------------------------------------
# Install kernel into ISO
# --------------------------------------------------

install: build/mykernel.bin
	mkdir -p iso/boot
	cp $< iso/boot/mykernel.bin

# --------------------------------------------------
# Create ISO
# --------------------------------------------------

$(ISO): install | $(BUILD_DIR)
	grub-mkrescue -o $(ISO) iso

# --------------------------------------------------
# Run QEMU
# --------------------------------------------------

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# --------------------------------------------------
# Run VirtualBox
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