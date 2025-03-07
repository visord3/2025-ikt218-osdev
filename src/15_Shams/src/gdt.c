#include "gdt.h"
#include "util.h"

// Use the renamed externs to match your assembly code
extern void gdt_flush(uint32_t);
extern void tss_flush();

segment_descriptor_t segment_descriptors[6];
descriptor_table_ptr_t descriptor_table_pointer;
task_state_segment_t task_segment;

void setup_global_descriptor_table()
{
    descriptor_table_pointer.size = (sizeof(struct segment_descriptor_struct) * 6) - 1;
    descriptor_table_pointer.address = (uint32_t)&segment_descriptors;

    create_descriptor_entry(0, 0, 0, 0, 0);                // Null segment
    create_descriptor_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
    create_descriptor_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
    create_descriptor_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
    create_descriptor_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment
    configure_task_segment(5, 0x10, 0x0);

    gdt_flush((uint32_t)&descriptor_table_pointer);
    tss_flush();
}

void create_descriptor_entry(uint32_t index, uint32_t base_addr, uint32_t limit, uint8_t access, uint8_t flags)
{
    segment_descriptors[index].base_low = (base_addr & 0xFFFF);
    segment_descriptors[index].base_middle = (base_addr >> 16) & 0xFF;
    segment_descriptors[index].base_high = (base_addr >> 24) & 0xFF;

    segment_descriptors[index].limit_low = (limit & 0xFFFF);
    segment_descriptors[index].limit_high_flags = (limit >> 16) & 0x0F;

    segment_descriptors[index].limit_high_flags |= flags & 0xF0;
    segment_descriptors[index].access_byte = access;
}

void configure_task_segment(uint32_t index, uint16_t kernel_ss, uint32_t kernel_esp)
{
    uint32_t base_addr = (uint32_t)&task_segment;
    uint32_t segment_limit = base_addr + sizeof(task_segment);

    create_descriptor_entry(index, base_addr, segment_limit, 0xE9, 0x00);
    memset(&task_segment, 0, sizeof(task_segment));

    task_segment.ss0 = kernel_ss;
    task_segment.esp0 = kernel_esp;

    task_segment.cs = 0x08 | 0x3;
    task_segment.ss = task_segment.ds = task_segment.es = task_segment.fs = task_segment.gs = 0x10 | 0x3;
}