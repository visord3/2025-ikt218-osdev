#include <libc/gdt.h>  // Include the file that defines the Global Descriptor Table (GDT)

// This function will be called to tell the processor about the new GDT location
extern void gdt_flush(uint32_t gdt_ptr);

// Function to set one specific entry (segment) in the GDT
void gdt_set_gate(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    
    // Set the starting address (base) of this segment
    gdt[index].base_low = base & 0xFFFF;           // Take the lowest 16 bits of the base address
    gdt[index].base_middle = (base >> 16) & 0xFF;  // Take the next 8 bits of the base address
    gdt[index].base_high = (base >> 24) & 0xFF;    // Take the highest 8 bits of the base address
    
    // Set how much memory this segment can cover (the segment limit)
    gdt[index].limit_low = limit & 0xFFFF;         // Take the lowest 16 bits of the limit
    gdt[index].granularity = (limit >> 16) & 0x0F; // Take the next 4 bits of the limit
    
    // Configure how the segment behaves (granularity and access level)
    gdt[index].granularity |= granularity & 0xF0;  // Set the size and granularity settings
    gdt[index].access = access;                    // Set the permissions for this segment (who can access it and how)
}

// Function to initialize and load the Global Descriptor Table (GDT)
void init_gdt() {
    
    // Calculate the size of the GDT (how many bytes it needs)
    gdt_ptr.limit = (sizeof(struct gdt_entry_t) * GDT_ENTRIES) - 1;
    
    // Set the location of the GDT in memory
    gdt_ptr.base = (uint32_t)&gdt;

    // Now, set up the different segments that the CPU will use

    // Null segment: This is a required entry in the GDT that does nothing
    gdt_set_gate(0, 0, 0, 0, 0);  // Always required and must be zero
    
    // Kernel code segment: This is for the kernel (the core part of the operating system) to execute code
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code that can access the entire memory
    
    // Kernel data segment: This is for the kernel to access and manage data
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data that can use the entire memory
    
    // User mode code segment: This is for user applications to execute their code
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // Code run by user programs
    
    // User mode data segment: This is for user applications to store and access their data
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // Data for user programs
    
    // Now, tell the CPU where the GDT is located using the gdt_flush function
    gdt_flush((uint32_t)&gdt_ptr);
}
