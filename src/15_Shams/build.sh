#!/bin/bash
set -e

# Define build directory
BUILD_DIR="/workspaces/2025-ikt218-osdev/build/15_Shams"
SRC_DIR="/workspaces/2025-ikt218-osdev/src/15_Shams"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake if cache doesn't exist
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo "Configuring CMake project..."
    cmake "$SRC_DIR"
fi

# Build the project
echo "Building kernel..."
cmake --build .

# Create the bootable ISO image
echo "Creating OS image..."
cmake --build . --target uiaos-create-image

echo "Build completed successfully!"
echo "You can run the OS with: qemu-system-i386 -cdrom $BUILD_DIR/kernel.iso"
