#include "types.h"
#include "screen.h"
#include "system.h"

// Minimal VGA dimensions for text mode:
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Simple kprint: writes the string to VGA text mode memory.
void kprint(const char *str)
{
    volatile char *vga = (volatile char *)0xb8000;
    while (*str)
    {
        *vga++ = *str++;
        *vga++ = 0x07; // attribute: light gray on black.
    }
}

// Clear the screen by filling VGA text mode buffer with spaces.
void clear_screen(void)
{
    volatile char *vga = (volatile char *)0xb8000;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x07;
    }
}

// Trigger software interrupt 0x80
void trigger_int80(void)
{
    asm volatile("int $0x80");
}

// Halt the CPU until next interrupt.
void cpu_halt(void)
{
    asm volatile("hlt");
}

// Minimal sprintf stub.
int sprintf(char *buffer, const char *format, ...)
{
    // Not fully implemented: return 0.
    (void)buffer;
    (void)format;
    return 0;
}

// Read one byte from an I/O port.
u8 port_byte_in(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Write one byte to an I/O port.
void port_byte_out(u16 port, u8 data)
{
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Enable interrupts.
void enable_interrupts(void)
{
    asm volatile("sti");
}
