#!/bin/bash
set -e

# Define directories
SRC_DIR="/workspaces/2025-ikt218-osdev/src/15_Shams"
BUILD_DIR="/workspaces/2025-ikt218-osdev/build/15_Shams"
LIMINE_DIR="/usr/local/limine"
LIMINE_CONFIG_FILE="${SRC_DIR}/limine.cfg"

echo "===== Starting Build Debugging ====="

# Check for required dependencies
echo "Checking dependencies..."
MISSING=0
DEPENDENCIES=("gcc" "g++" "nasm" "cmake" "xorriso" "dd" "mkfs.fat")

for dep in "${DEPENDENCIES[@]}"; do
    if ! command -v $dep &> /dev/null; then
        echo "❌ Missing dependency: $dep"
        MISSING=1
    else
        echo "✅ Found dependency: $dep"
    fi
done

# Check Limine files
echo "Checking Limine bootloader files..."
if [ ! -d "$LIMINE_DIR" ]; then
    echo "❌ Limine directory not found at ${LIMINE_DIR}"
    echo "Installing Limine..."
    
    # Create a temporary directory
    TMP_DIR=$(mktemp -d)
    cd "$TMP_DIR"
    
    # Clone and build Limine
    git clone https://github.com/limine-bootloader/limine.git --branch=v4.x-branch-binary --depth=1
    cd limine
    make
    
    # Create Limine directory if it doesn't exist
    sudo mkdir -p "$LIMINE_DIR"
    
    # Copy Limine files
    sudo cp limine-bios.sys limine-bios-cd.bin limine-uefi-cd.bin "$LIMINE_DIR/"
    sudo cp BOOTX64.EFI BOOTIA32.EFI "$LIMINE_DIR/"
    
    # Install limine utility
    sudo cp bin/limine "$LIMINE_DIR/"
    sudo chmod +x "$LIMINE_DIR/limine"
    
    cd "$SRC_DIR"
    rm -rf "$TMP_DIR"
else
    echo "✅ Limine directory found"
    
    # Check specific Limine files
    for file in "limine-bios.sys" "limine-bios-cd.bin" "limine-uefi-cd.bin" "BOOTX64.EFI" "BOOTIA32.EFI" "limine"; do
        if [ ! -f "${LIMINE_DIR}/${file}" ]; then
            echo "❌ Missing Limine file: ${file}"
            MISSING=1
        else
            echo "✅ Found Limine file: ${file}"
        fi
    done
fi

# Check for limine.cfg
if [ ! -f "$LIMINE_CONFIG_FILE" ]; then
    echo "❌ limine.cfg not found at ${LIMINE_CONFIG_FILE}, creating one..."
    cat > "$LIMINE_CONFIG_FILE" << EOL
# Timeout in seconds for the bootloader to choose the default entry.
TIMEOUT=5

# The entry name that will be displayed in the boot menu.
:UiA Operating System
    # Configure the kernel protocol we want to boot with.
    PROTOCOL=multiboot2
    
    # Path to the kernel to boot.
    KERNEL_PATH=boot:///kernel.bin
EOL
    echo "✅ Created limine.cfg"
else
    echo "✅ limine.cfg found"
fi

# Check linker script
LINKER_FILE="${SRC_DIR}/src/arch/i386/linker.ld"
if [ ! -f "$LINKER_FILE" ]; then
    echo "❌ Linker script not found at ${LINKER_FILE}, creating directories and file..."
    mkdir -p "${SRC_DIR}/src/arch/i386"
    
    cat > "$LINKER_FILE" << EOL
/* Linker script for i386 architecture */
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    /* Load the kernel at 1MB, a common location for kernels */
    . = 1M;
    
    /* First put the multiboot header, as it needs to be at the start of the binary */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
}
EOL
    echo "✅ Created linker script"
else
    echo "✅ Linker script found"
fi

# Clean the build directory
echo "Cleaning build directory..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure and build with verbose output
echo "Configuring CMake project..."
cd "$BUILD_DIR"
cmake -DCMAKE_VERBOSE_MAKEFILE=ON "$SRC_DIR"

echo "Building kernel..."
cmake --build . -v

echo "Creating OS image..."
cmake --build . --target uiaos-create-image -v

# Check if ISO was created
if [ -f "$BUILD_DIR/kernel.iso" ]; then
    echo "✅ ISO file created successfully! ${BUILD_DIR}/kernel.iso"
    
    # Run QEMU if requested
    if [ "$1" = "run" ]; then
        echo "Running OS in QEMU..."
        qemu-system-i386 -cdrom "$BUILD_DIR/kernel.iso"
    fi
else
    echo "❌ ISO file not created. Check the build logs above."
    
    # Look for any kernel.iso file
    ISO_FOUND=$(find "$BUILD_DIR" -name "kernel.iso" | head -n 1)
    if [ -n "$ISO_FOUND" ]; then
        echo "✅ Found ISO file at alternate location: $ISO_FOUND"
    fi
fi

echo "===== Build Debugging Completed ====="
