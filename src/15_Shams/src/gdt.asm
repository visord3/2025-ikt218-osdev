[GLOBAL gdt_flush]    ; Let C programs call this function.

gdt_flush:
    mov eax, [esp+4]  ; Get the GDT pointer (C program gives this as input).
    
    lgdt [eax]        ; Load the GDT address into the CPU (tell the CPU to use this GDT).
    
    ; Update CPU settings to use the new memory layout from the GDT.
    
    mov ax, 0x10      ; 0x10 is the starting point of the data segment in the GDT.
    mov ds, ax        ; Set where regular data is stored.
    mov es, ax        ; Set where extra data is stored.
    mov fs, ax        ; Set special data storage.
    mov gs, ax        ; Set another special data area.
    mov ss, ax        ; Set where the stack (temporary data) is stored.

    ; Jump to the new code segment to start using the updated GDT.
    jmp 0x08:.flush   ; 0x08 is the starting point of the code segment in the GDT.

.flush:
    ret               ; Go back to the C program that called this function.

[GLOBAL tss_flush]    ; Export the function for C code to use

tss_flush:
    mov ax, 0x2B      ; 0x2B is the TSS entry (index 5 × 8 + 3)
    ltr ax            ; Load Task Register with the segment selector
    ret               ; Return to C code