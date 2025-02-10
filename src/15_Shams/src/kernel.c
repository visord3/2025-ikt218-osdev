#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdint.h"
#include "libc/stddef.h"


// Terminal output
volatile uint16_t* video_memory = (uint16_t*)0xB8000;
int cursor_x = 0, cursor_y = 0;


// Write a single character to the screen
void terminal_putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        video_memory[cursor_y * 80 + cursor_x] = (0x0F << 8) | c;
        cursor_x++;
    }
}

// Write a string to the screen
void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putc(str[i]);
    }
}

void check_gdt() {
    uint16_t cs, ds;
    __asm__ __volatile__ ("mov %%cs, %0" : "=r"(cs));  
    __asm__ __volatile__ ("mov %%ds, %0" : "=r"(ds)); 

    terminal_write("\nChecking GDT:\n");
    if (cs == 0x08 && ds == 0x10) {
        terminal_write("GDT Loaded Successfully\n");
    } else {
        terminal_write("GDT Failed\n");
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    // Print "Hello World" to the screen
    terminal_write("Hello World\n");
init_gdt();
    check_gdt();
    return 0;
}
