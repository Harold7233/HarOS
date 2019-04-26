#include <x86/gdt.h>
#include <task/task.h>
#include <libc/string.h>
gdt_ptr_t gdt_ptr;
gdt_entry_t gdt_table[GDT_TABLE_SIZE];
tss_t tss_entry;
extern void load_gdt(gdt_ptr_t *gdt_base_addr);

void set_gdt_entry(u8int_t index, u32int_t base, u32int_t limit, u8int_t access, u8int_t granularity)
{
    gdt_entry_t *entry = &gdt_table[index];
    entry->base_high = base >> 24 & 0xFF;
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base & (0xFF << 16)) >> 16;
    entry->limit_low = (limit & 0xFFFF);
    entry->granularity = (limit >> 16) & 0x0F;
    entry->granularity |= granularity & 0xF0;
    entry->access = access;
}

void init_gdt()
{

    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_TABLE_SIZE) - 1;
    gdt_ptr.base = (u32int_t)&gdt_table;
    set_gdt_entry(0, 0, 0, 0, 0);                // 空描述符
    set_gdt_entry(KERNEL_CS_GDT_INDEX, 0, 0xFFFFFFFF, 0x9A, 0xCF); // 内核代码段
    set_gdt_entry(KERNEL_DS_GDT_INDEX, 0, 0xFFFFFFFF, 0x92, 0xCF); // 内核数据段
    set_gdt_entry(USER_CS_GDT_INDEX, 0, 0xFFFFFFFF, 0xFA, 0xCF); // 用户代码段
    set_gdt_entry(USER_DS_GDT_INDEX, 0, 0xFFFFFFFF, 0xF2, 0xCF); // 用户数据段
    init_tss(0x10, 0); // tss
    load_gdt(&gdt_ptr);
}

void init_tss(u16int_t ss0, u16int_t esp0)
{
    u32int_t base = (u32int_t)&tss_entry;
    u32int_t limit = base + sizeof(tss_entry);

    set_gdt_entry(5, base, limit, 0xE9, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;   
    tss_entry.esp0 = esp0; 

    tss_entry.cs = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}
