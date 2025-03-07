# Compiler/Assembler options
CC = gcc
AS = as
LD = ld
CFLAGS = -m32 -fno-builtin -fno-stack-protector -nostdinc -O0 -g -Wall -Wextra -std=gnu99 -I./include
ASFLAGS = --32
LDFLAGS = -melf_i386 -T linker.ld -nostdlib

# Source files
C_SOURCES = $(wildcard src/*.c)
ASM_SOURCES = src/boot.s
OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

# Output files
KERNEL = mykernel.bin
ISO = myos.iso

all: $(KERNEL) $(ISO)

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	echo "menuentry 'My OS' {" > iso/boot/grub/grub.cfg
	echo "   multiboot /boot/$(KERNEL)" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M

debug: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M -s -S &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL)"

clean:
	rm -rf $(OBJ) $(KERNEL) $(ISO) iso

.PHONY: all clean run debug
