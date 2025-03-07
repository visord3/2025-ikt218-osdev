#!/bin/bash

# Remove the corrupted build directory
rm -rf /workspaces/2025-ikt218-osdev/build/15_Shams

# Create a fresh build directory
mkdir -p /workspaces/2025-ikt218-osdev/build/15_Shams

# Run CMake configuration from scratch
cd /workspaces/2025-ikt218-osdev/build/15_Shams
cmake /workspaces/2025-ikt218-osdev/src/15_Shams

# Build the project
cmake --build /workspaces/2025-ikt218-osdev/build/15_Shams --target uiaos-create-image
