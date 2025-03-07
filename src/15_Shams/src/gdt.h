#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"

struct segment_descriptor_struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access_byte;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((packed));

typedef struct segment_descriptor_struct segment_descriptor_t;

struct descriptor_table_ptr_struct
{
    uint16_t size;
    uint32_t address;
} __attribute__((packed));

typedef struct descriptor_table_ptr_struct descriptor_table_ptr_t;

struct task_state_segment_struct
{
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

typedef struct task_state_segment_struct task_state_segment_t;

void setup_global_descriptor_table(void);
void create_descriptor_entry(uint32_t index, uint32_t base_addr, uint32_t limit, uint8_t access, uint8_t flags);
void configure_task_segment(uint32_t index, uint16_t kernel_ss, uint32_t kernel_esp);

#endif