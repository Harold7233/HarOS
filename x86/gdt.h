#include <common/common.h>
#ifndef GDT_H
#define GDT_H
#define GDT_TABLE_SIZE 6
#define GDT_SEGMENT_LEN 0xFFFFFFFF
#define KERNEL_CS_GDT_INDEX 1
#define KERNEL_DS_GDT_INDEX 2
#define USER_CS_GDT_INDEX 3
#define USER_DS_GDT_INDEX 4
typedef struct gdt_entry_struct
{
    u16int_t limit_low;  // limit 0-15位
    u16int_t base_low;   // base 0-15位
    u8int_t base_middle; // base 16-23位
    u8int_t access;      // access flags
    u8int_t granularity;
    u8int_t base_high; // base 24-31位
} __attribute__((packed)) gdt_entry_t;

// 读入GDTR寄存器的内容，共48位
typedef struct gdt_ptr_struct
{
    u16int_t limit; // gdt表长度-1
    u32int_t base;  // gdt表基址
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();
void set_gdt_entry(u8int_t index, u32int_t base, u32int_t limit, u8int_t access, u8int_t granularity);
#endif