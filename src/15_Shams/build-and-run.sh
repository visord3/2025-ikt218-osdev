#!/bin/bash
set -e

# Define directories
SRC_DIR="/workspaces/2025-ikt218-osdev/src/15_Shams"
BUILD_DIR="/workspaces/2025-ikt218-osdev/build/15_Shams"
ISO_FILE="${BUILD_DIR}/kernel.iso"

# Function for error handling
error_exit() {
    echo "ERROR: $1" >&2
    exit 1
}

# Create build directory
echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || error_exit "Failed to change to build directory"

# Clean any previous build artifacts if requested
if [ "$1" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"/*
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" || error_exit "Failed to change to build directory after clean"
fi

# Verify CMake is installed
if ! command -v cmake &> /dev/null; then
    error_exit "CMake is not installed. Please install CMake to continue."
fi

# Configure the project with CMake
echo "Configuring CMake project..."
cmake "$SRC_DIR" || error_exit "CMake configuration failed"

# Build the project
echo "Building kernel..."
cmake --build . || error_exit "Kernel build failed"

# Create the bootable ISO image
echo "Creating OS image..."
cmake --build . --target uiaos-create-image || error_exit "ISO image creation failed"

# Verify ISO was created
if [ ! -f "$ISO_FILE" ]; then
    # Try to locate the ISO file anywhere in the build directory
    ISO_FOUND=$(find "$BUILD_DIR" -name "kernel.iso" | head -n 1)
    
    if [ -n "$ISO_FOUND" ]; then
        echo "ISO file found at alternate location: $ISO_FOUND"
        ISO_FILE=$ISO_FOUND
    else
        error_exit "ISO file was not created. Check CMake target configuration."
    fi
fi

echo "Build completed successfully!"
echo "ISO file: $ISO_FILE"

# Run in QEMU
if [ "$1" != "build-only" ]; then
    echo "Running OS in QEMU..."
    qemu-system-i386 -cdrom "$ISO_FILE" || error_exit "QEMU failed to start"
fi
