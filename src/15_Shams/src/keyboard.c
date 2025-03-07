#include "libc/stdint.h"
#include "util.h"
#include "vga.h"
#include "libc/idt.h"

// US keyboard scancode-to-ASCII lookup table
unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, '<', 0, 0, 0, 0, 0};

// Buffer for storing keyboard input
#define KEYBOARD_BUFFER_SIZE 256
char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
uint32_t buffer_position = 0;

void keyboard_handler(struct InterruptRegisters *regs)
{
    // Read from keyboard's data buffer
    uint8_t scancode = port_byte_in(0x60);

    // Only process the key if it's pressed down (not released)
    if (scancode & 0x80)
    {
        // Key was released, we ignore these events
        return;
    }

    // Translate the scancode to ASCII
    char c = keyboard_map[scancode];

    // Print character only if it's printable
    if (c != 0)
    {
        // Print the character to screen
        char str[2] = {c, '\0'};
        terminal_write(str);

        // Store in buffer if there's space
        if (buffer_position < KEYBOARD_BUFFER_SIZE - 1)
        {
            keyboard_buffer[buffer_position++] = c;
            keyboard_buffer[buffer_position] = '\0'; // Null terminate
        }
    }
}

void initialize_keyboard()
{
    // Clear buffer
    for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
    {
        keyboard_buffer[i] = 0;
    }

        irq_install_handler(1, keyboard_handler);

    terminal_write("Keyboard initialized\n");
}