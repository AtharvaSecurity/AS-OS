CXX = g++
ASM = nasm
LD = ld

CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-rtti -fno-exceptions -ffreestanding -Wall -Wextra -O2 -I./include
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

CXX_SOURCES = drivers/graphics.cpp kernel/kernel.cpp drivers/terminal.cpp drivers/keyboard.cpp drivers/cursor.cpp drivers/gdt.cpp drivers/isr.cpp drivers/memory.cpp drivers/process.cpp drivers/fs.cpp drivers/auth.cpp drivers/network.cpp drivers/gui.cpp drivers/packages.cpp drivers/calculator.cpp drivers/apps.cpp drivers/mouse.cpp

CXX_OBJECTS = build/graphics.o build/kernel.o build/terminal.o build/keyboard.o build/cursor.o build/gdt.o build/isr.o build/memory.o build/process.o build/fs.o build/auth.o build/network.o build/gui.o build/packages.o build/calculator.o build/apps.o build/mouse.o

ASM_OBJECTS = build/boot.o

KERNEL_BIN = build/kernel.bin
ISO_FILE = build/as-os.iso

all: $(KERNEL_BIN)

build/boot.o: arch/i386/boot.asm
	@mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@

build/%.o: kernel/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: drivers/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(KERNEL_BIN): $(ASM_OBJECTS) $(CXX_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "========================================"
	@echo "  AS OS v10.0 MEGA - BUILD COMPLETE!"
	@echo "========================================"

iso: $(KERNEL_BIN)
	@mkdir -p iso/boot/grub
	cp $(KERNEL_BIN) iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) iso 2>/dev/null
	@echo "  ISO: $(ISO_FILE)"

run: iso
	qemu-system-i386 -cdrom $(ISO_FILE)

clean:
	rm -rf build/* iso/boot
	@echo "  Cleaned"

.PHONY: all iso run clean
