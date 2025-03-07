void memset(void *dest, char val, uint32_t count);
void outPortB(uint16_t Port, uint8_t Value);
uint8_t port_byte_in(uint16_t Port);

struct InterruptRegisters
{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

// Add this for multiboot
struct multiboot_info
{
    /* Define necessary fields based on your multiboot implementation */
    uint32_t flags;
    /* Add other fields as needed */
};