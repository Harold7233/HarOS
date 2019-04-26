#include <x86/idt.h>
#include <x86/interrupt.h>
idt_entry_t idt_table[256];
idt_ptr_t idt_ptr;

static void set_idt_entry(u8int_t index, u32int_t base, u16int_t sel, u8int_t flags)
{
    idt_entry_t *entry = &idt_table[index];
    entry->base_lo = base & 0xFFFF;
    entry->base_hi = (base >> 16) & 0xFFFF;
    entry->sel = sel;
    entry->dummy = 0;
    // TODO 用户态取消下面的注释
    entry->flags = flags /* | 0x60 */;
}

void init_idt()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    idt_ptr.limit = sizeof(idt_table) - 1;
    idt_ptr.base = (u32int_t)&idt_table;
    set_idt_entry(0, (u32int_t)isr0, 0x08, 0x8E);
    set_idt_entry(1, (u32int_t)isr1, 0x08, 0x8E);
    set_idt_entry(2, (u32int_t)isr2, 0x08, 0x8E);
    set_idt_entry(3, (u32int_t)isr3, 0x08, 0x8E);
    set_idt_entry(4, (u32int_t)isr4, 0x08, 0x8E);
    set_idt_entry(5, (u32int_t)isr5, 0x08, 0x8E);
    set_idt_entry(6, (u32int_t)isr6, 0x08, 0x8E);
    set_idt_entry(7, (u32int_t)isr7, 0x08, 0x8E);
    set_idt_entry(8, (u32int_t)isr8, 0x08, 0x8E);
    set_idt_entry(9, (u32int_t)isr9, 0x08, 0x8E);
    set_idt_entry(10, (u32int_t)isr10, 0x08, 0x8E);
    set_idt_entry(11, (u32int_t)isr11, 0x08, 0x8E);
    set_idt_entry(12, (u32int_t)isr12, 0x08, 0x8E);
    set_idt_entry(13, (u32int_t)isr13, 0x08, 0x8E);
    set_idt_entry(14, (u32int_t)isr14, 0x08, 0x8E);
    set_idt_entry(15, (u32int_t)isr15, 0x08, 0x8E);
    set_idt_entry(16, (u32int_t)isr16, 0x08, 0x8E);
    set_idt_entry(17, (u32int_t)isr17, 0x08, 0x8E);
    set_idt_entry(18, (u32int_t)isr18, 0x08, 0x8E);
    set_idt_entry(19, (u32int_t)isr19, 0x08, 0x8E);
    set_idt_entry(20, (u32int_t)isr20, 0x08, 0x8E);
    set_idt_entry(21, (u32int_t)isr21, 0x08, 0x8E);
    set_idt_entry(22, (u32int_t)isr22, 0x08, 0x8E);
    set_idt_entry(23, (u32int_t)isr23, 0x08, 0x8E);
    set_idt_entry(24, (u32int_t)isr24, 0x08, 0x8E);
    set_idt_entry(25, (u32int_t)isr25, 0x08, 0x8E);
    set_idt_entry(26, (u32int_t)isr26, 0x08, 0x8E);
    set_idt_entry(27, (u32int_t)isr27, 0x08, 0x8E);
    set_idt_entry(28, (u32int_t)isr28, 0x08, 0x8E);
    set_idt_entry(29, (u32int_t)isr29, 0x08, 0x8E);
    set_idt_entry(30, (u32int_t)isr30, 0x08, 0x8E);
    set_idt_entry(31, (u32int_t)isr31, 0x08, 0x8E);
    set_idt_entry(32, (u32int_t)irq0, 0x08, 0x8E);
    set_idt_entry(33, (u32int_t)irq1, 0x08, 0x8E);
    set_idt_entry(34, (u32int_t)irq2, 0x08, 0x8E);
    set_idt_entry(35, (u32int_t)irq3, 0x08, 0x8E);
    set_idt_entry(36, (u32int_t)irq4, 0x08, 0x8E);
    set_idt_entry(37, (u32int_t)irq5, 0x08, 0x8E);
    set_idt_entry(38, (u32int_t)irq6, 0x08, 0x8E);
    set_idt_entry(39, (u32int_t)irq7, 0x08, 0x8E);
    set_idt_entry(40, (u32int_t)irq8, 0x08, 0x8E);
    set_idt_entry(41, (u32int_t)irq9, 0x08, 0x8E);
    set_idt_entry(42, (u32int_t)irq10, 0x08, 0x8E);
    set_idt_entry(43, (u32int_t)irq11, 0x08, 0x8E);
    set_idt_entry(44, (u32int_t)irq12, 0x08, 0x8E);
    set_idt_entry(45, (u32int_t)irq13, 0x08, 0x8E);
    set_idt_entry(46, (u32int_t)irq14, 0x08, 0x8E);
    set_idt_entry(47, (u32int_t)irq15, 0x08, 0x8E);

    set_idt_entry(0x80, (u32int_t)isr128, 0x08, 0x8E);

    load_idt((u32int_t)&idt_ptr);
}
