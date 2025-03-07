ENTRY(_start)

SECTIONS {
    /* Start at 1MB which is standard for i386 */
    . = 0x100000;

    /* Put multiboot header first so GRUB can find it */
    .text : {
        *(.multiboot)  /* Multiboot header must come first */
        *(.text)       /* Then the regular code */
    }

    /* Read-only data */
    .rodata : {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data : {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss : {
        *(COMMON)
        *(.bss)
    }
}
