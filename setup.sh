#!/bin/bash
set -e

# Create necessary directories
mkdir -p src include

# Ensure permissions are correct
chmod +x build.sh
chmod +x run.sh

# Remove unnecessary files
echo "Cleaning unnecessary files..."
rm -rf src/15_Shams 2>/dev/null || true
rm -rf cmake 2>/dev/null || true

# Move boot.S if it's in the wrong location
if [ -f "boot.S" ] && [ ! -f "src/boot.S" ]; then
    mv boot.S src/
fi

# Create include directory if it doesn't exist
if [ ! -d "include" ]; then
    mkdir include
fi

echo "Directory structure setup completed!"
echo "Source files should be in src/"
echo "Header files should be in include/"
echo "Run ./build.sh to build the project"
