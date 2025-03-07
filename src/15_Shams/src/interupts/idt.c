#include "libc/stdint.h"
#include "../util.h"
#include "vga.h"
#include "libc/idt.h"

// Declaration for the interrupt handlers
void division_by_zero_handler(struct InterruptRegisters *regs);
void debug_exception_handler(struct InterruptRegisters *regs);
void breakpoint_handler(struct InterruptRegisters *regs);

#define IDT_ENTRIES 256
void (*interrupt_handlers[IDT_ENTRIES])(struct InterruptRegisters *);

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);

void configure_interrupt_system()
{
    // Setup IDT pointer
    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Clear the IDT
    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

    // Remap the PIC to avoid conflicts with CPU exceptions
    outPortB(0x20, 0x11);
    outPortB(0xA0, 0x11);
    outPortB(0x21, 0x20);
    outPortB(0xA1, 0x28);
    outPortB(0x21, 0x04);
    outPortB(0xA1, 0x02);
    outPortB(0x21, 0x01);
    outPortB(0xA1, 0x01);
    outPortB(0x21, 0x0);
    outPortB(0xA1, 0x0);

    // Set up exception handlers
    setIdtGate(0, (uint32_t)isr0, 0x08, 0x8E);
    setIdtGate(1, (uint32_t)isr1, 0x08, 0x8E);
    setIdtGate(2, (uint32_t)isr2, 0x08, 0x8E);
    setIdtGate(3, (uint32_t)isr3, 0x08, 0x8E);
    setIdtGate(4, (uint32_t)isr4, 0x08, 0x8E);
    setIdtGate(5, (uint32_t)isr5, 0x08, 0x8E);
    setIdtGate(6, (uint32_t)isr6, 0x08, 0x8E);
    setIdtGate(7, (uint32_t)isr7, 0x08, 0x8E);
    setIdtGate(8, (uint32_t)isr8, 0x08, 0x8E);
    setIdtGate(9, (uint32_t)isr9, 0x08, 0x8E);
    setIdtGate(10, (uint32_t)isr10, 0x08, 0x8E);
    setIdtGate(11, (uint32_t)isr11, 0x08, 0x8E);
    setIdtGate(12, (uint32_t)isr12, 0x08, 0x8E);
    setIdtGate(13, (uint32_t)isr13, 0x08, 0x8E);
    setIdtGate(14, (uint32_t)isr14, 0x08, 0x8E);
    setIdtGate(15, (uint32_t)isr15, 0x08, 0x8E);
    setIdtGate(16, (uint32_t)isr16, 0x08, 0x8E);
    setIdtGate(17, (uint32_t)isr17, 0x08, 0x8E);
    setIdtGate(18, (uint32_t)isr18, 0x08, 0x8E);
    setIdtGate(19, (uint32_t)isr19, 0x08, 0x8E);
    setIdtGate(20, (uint32_t)isr20, 0x08, 0x8E);
    setIdtGate(21, (uint32_t)isr21, 0x08, 0x8E);
    setIdtGate(22, (uint32_t)isr22, 0x08, 0x8E);
    setIdtGate(23, (uint32_t)isr23, 0x08, 0x8E);
    setIdtGate(24, (uint32_t)isr24, 0x08, 0x8E);
    setIdtGate(25, (uint32_t)isr25, 0x08, 0x8E);
    setIdtGate(26, (uint32_t)isr26, 0x08, 0x8E);
    setIdtGate(27, (uint32_t)isr27, 0x08, 0x8E);
    setIdtGate(28, (uint32_t)isr28, 0x08, 0x8E);
    setIdtGate(29, (uint32_t)isr29, 0x08, 0x8E);
    setIdtGate(30, (uint32_t)isr30, 0x08, 0x8E);
    setIdtGate(31, (uint32_t)isr31, 0x08, 0x8E);

    setIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);
    setIdtGate(33, (uint32_t)irq1, 0x08, 0x8E);
    setIdtGate(34, (uint32_t)irq2, 0x08, 0x8E);
    setIdtGate(35, (uint32_t)irq3, 0x08, 0x8E);
    setIdtGate(36, (uint32_t)irq4, 0x08, 0x8E);
    setIdtGate(37, (uint32_t)irq5, 0x08, 0x8E);
    setIdtGate(38, (uint32_t)irq6, 0x08, 0x8E);
    setIdtGate(39, (uint32_t)irq7, 0x08, 0x8E);
    setIdtGate(40, (uint32_t)irq8, 0x08, 0x8E);
    setIdtGate(41, (uint32_t)irq9, 0x08, 0x8E);
    setIdtGate(42, (uint32_t)irq10, 0x08, 0x8E);
    setIdtGate(43, (uint32_t)irq11, 0x08, 0x8E);
    setIdtGate(44, (uint32_t)irq12, 0x08, 0x8E);
    setIdtGate(45, (uint32_t)irq13, 0x08, 0x8E);
    setIdtGate(46, (uint32_t)irq14, 0x08, 0x8E);
    setIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);

    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E); // System calls
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E); // System calls

    // Register specific handlers for our test interrupts
    interrupt_handlers[0] = division_by_zero_handler;
    interrupt_handlers[1] = debug_exception_handler;
    interrupt_handlers[3] = breakpoint_handler;

    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
}

void setIdtGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{

    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags | 0x60;
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

};

// handlers after your exception_messages array
void division_by_zero_handler(struct InterruptRegisters *regs)
{
    terminal_write("ISR 0: Division By Zero Exception!\n");
}

void debug_exception_handler(struct InterruptRegisters *regs)
{
    terminal_write("ISR 1: Debug Exception!\n");
}

void breakpoint_handler(struct InterruptRegisters *regs)
{
    terminal_write("ISR 3: Breakpoint Exception!\n");
}

void isr_handler(struct InterruptRegisters *regs)
{
    if (interrupt_handlers[regs->int_no])
    {
        interrupt_handlers[regs->int_no](regs);
    }
    else
    {
        char int_str[20];
        // Convert int number to string
        int_str[0] = 'I';
        int_str[1] = 'N';
        int_str[2] = 'T';
        int_str[3] = ' ';
        int_str[4] = '0';
        int_str[5] = '0';
        int_str[6] = 'x';
        // Convert hex value
        int value = regs->int_no;
        int pos = 7;

        if (value == 0)
        {
            int_str[pos++] = '0';
        }
        else
        {
            char hex_chars[] = "0123456789ABCDEF";
            char hex_str[10];
            int hex_pos = 0;

            while (value > 0)
            {
                hex_str[hex_pos++] = hex_chars[value % 16];
                value /= 16;
            }

            // Reverse and copy
            for (int i = hex_pos - 1; i >= 0; i--)
            {
                int_str[pos++] = hex_str[i];
            }
        }

        int_str[pos++] = ':';
        int_str[pos++] = ' ';
        int_str[pos++] = 'U';
        int_str[pos++] = 'n';
        int_str[pos++] = 'h';
        int_str[pos++] = 'a';
        int_str[pos++] = 'n';
        int_str[pos++] = 'd';
        int_str[pos++] = 'l';
        int_str[pos++] = 'e';
        int_str[pos++] = 'd';
        int_str[pos++] = '\n';
        int_str[pos] = '\0';

        terminal_write(int_str);
    }
}

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters *regs)
{
    void (*handler)(struct InterruptRegisters *regs);

    handler = irq_routines[regs->int_no - 32];

    if (handler)
    {
        handler(regs);
    }

    if (regs->int_no >= 40)
    {
        outPortB(0xA0, 0x20);
    }

    outPortB(0x20, 0x20);
}