#!/bin/bash
set -e

# Build the project
./build.sh

# Run in QEMU
echo "Running in QEMU..."
qemu-system-i386 -cdrom build/kernel.iso
