#include "libc/stdbool.h"
#include "multiboot2.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/keyboard.h"
#include "gdt.h"
#include "libc/idt.h"

volatile uint16_t *display_buffer = (uint16_t *)0xB8000;
int display_pos_x = 0, display_pos_y = 0;

void display_character(char c)
{
    if (c == '\n')
    {
        display_pos_x = 0;
        display_pos_y++;
    }
    else
    {
        display_buffer[display_pos_y * 80 + display_pos_x] = (0x0F << 8) | c;
        display_pos_x++;
    }
}

void display_text(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        display_character(str[i]);
    }
}

// Add display initialization function
void display_init(void)
{
    // Clear screen
    for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            display_buffer[y * 80 + x] = (0x0F << 8) | ' ';
        }
    }
    display_pos_x = 0;
    display_pos_y = 0;
}

void verify_segment_setup(void)
{
    uint16_t code_seg, data_seg;
    __asm__ __volatile__("mov %%cs, %0" : "=r"(code_seg));
    __asm__ __volatile__("mov %%ds, %0" : "=r"(data_seg));

    display_text("\nVerifying memory segments:\n");
    if (code_seg == 0x08 && data_seg == 0x10)
    {
        display_text("Memory segments configured correctly\n");
    }
    else
    {
        display_text("Memory segment configuration error\n");
    }
}

void terminal_write(const char *str)
{
    display_text(str);
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{

    display_init();
    display_text("ShamsOS Kernel Started\n");

    display_text("Configuring memory segments...\n");
    setup_global_descriptor_table();
    display_text("Memory segments and TSS initialized\n");

    display_text("Setting up interrupt handlers...\n");
    configure_interrupt_system();

    // Enable hardware interrupts
    asm volatile("sti");

    display_text("Initializing input devices...\n");
    initialize_keyboard();

    display_text("System ready, keyboard active:\n");

    // Test exception handlers
    display_text("\nTesting exception handler: Division Error\n");
    asm volatile("int $0x0");

    display_text("\nTesting exception handler: Debug Exception\n");
    asm volatile("int $0x1");

    display_text("\nTesting exception handler: Breakpoint\n");
    asm volatile("int $0x3");

    display_text("\nAll exception handlers functioning properly\n");

    // System idle loop
    while (1)
    {
        asm volatile("hlt");
    }
    return 0;
}