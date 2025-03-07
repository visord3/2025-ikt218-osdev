ENTRY(_start)

SECTIONS {
    /* Put multiboot header first */
    . = 1M;
    .multiboot_header :
    {
        *(.multiboot_header)
    }

    /* Entry point */
    ENTRY(_start)

    . = ALIGN(4K);
    .text :
    {
        *(.text .text.*)
    }

    . = ALIGN(4K);
    .rodata :
    {
        *(.rodata .rodata.*)
    }

    . = ALIGN(4K);
    .data :
    {
        *(.data .data.*)
    }

    . = ALIGN(4K);
    .bss :
    {
        *(.bss .bss.*)
    }

	end = .; _end = .; __end = .;
}