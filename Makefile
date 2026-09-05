CC = gcc

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -Iinclude
ASFLAGS = --32
LDFLAGS = -melf_i386

BUILD_DIR = build
ISO = $(BUILD_DIR)/myos.iso

VM_NAME = MyOS

# --------------------------------------------------
# Object files
# --------------------------------------------------
OBJECTS = build/loader.o build/kernel.o build/gdt.o build/port.o build/interruptstub.o build/interrupts.o build/keyboard.o build/mouse.o build/driver.o

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

build/kernel.o: src/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Driver
# --------------------------------------------------

build/driver.o: src/driver.c include/driver/driver.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# C files inside their own directory
#
# gdt/gdt.c -> build/gdt.o
# idt/idt.c -> build/idt.o
# paging/paging.c -> build/paging.o
# --------------------------------------------------

build/gdt.o: src/gdt/gdt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Port
# --------------------------------------------------

build/port.o: src/port/port.c include/hardwarecommunication/port.h src/port/port8.c src/port/port8Slow.c src/port/port16.c src/port/port32.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Keyboard
# --------------------------------------------------

build/keyboard.o:	src/hardware/keyboard/keyboard.c include/driver/keyboard.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Mouse
# --------------------------------------------------

build/mouse.o:	src/hardware/mouse/mouse.c include/driver/mouse.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Interrupts
# --------------------------------------------------

build/interrupts.o: src/interrupts/interrupts.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

build/interruptstub.o: src/interrupts/interruptstubs.s | $(BUILD_DIR)
	as $(ASFLAGS) -o $@ $<


# --------------------------------------------------
# Assembly
# --------------------------------------------------

build/loader.o: src/loader.s | $(BUILD_DIR)
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