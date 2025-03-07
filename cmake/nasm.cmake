# Configure NASM assembly language support

# Find NASM
find_program(CMAKE_ASM_NASM_COMPILER NAMES nasm)
if(NOT CMAKE_ASM_NASM_COMPILER)
    message(FATAL_ERROR "NASM assembler not found")
endif()

# Set proper NASM flags for 32-bit x86
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS asm;nasm)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf32 -o <OBJECT> <SOURCE>")
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g -F dwarf")
enable_language(ASM_NASM)
