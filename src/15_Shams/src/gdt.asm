[GLOBAL gdt_flush]    ; Make gdt_flush available to C programs that need to call it.

gdt_flush:
    mov eax, [esp+4]  ; Get the GDT pointer (the parameter passed from C is stored at esp+4)
    
    lgdt [eax]        ; Load the address of the new GDT into the CPU using the 'lgdt' instruction
    
    ; Now, we need to update the segment registers to point to the correct segments in the new GDT
    
    mov ax, 0x10      ; 0x10 is the offset of the data segment in our GDT
    mov ds, ax        ; Set the data segment (DS) to point to the new data segment
    mov es, ax        ; Set the extra segment (ES) to the data segment as well
    mov fs, ax        ; Set the FS segment (for specific data access) to the new data segment
    mov gs, ax        ; Set the GS segment (used in some specialized cases) to the new data segment
    mov ss, ax        ; Set the stack segment (SS) to the new data segment

    ; Perform a far jump to update the code segment (CS) to the new GDT entry for code execution
    jmp 0x08:.flush   ; 0x08 is the offset for the code segment. This jump ensures the CPU uses the new GDT.

.flush:
    ret               ; Return to the C code that called this function
