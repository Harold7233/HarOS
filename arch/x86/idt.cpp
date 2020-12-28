#include <arch/x86/idt.hpp>

#include <sys/asm.hpp>
#include <arch/x86/io.hpp>
#include <arch/x86/interrupt.hpp>
x86::idt_entry_t idt_table[256];
x86::idt_ptr_t idt_ptr;

void set_idt_entry(uint8_t index, uint32_t base, uint16_t sel, uint8_t flags) {
    x86::idt_entry_t *entry = &idt_table[index];
    entry->base_lo = base & 0xFFFF;
    entry->base_hi = (base >> 16) & 0xFFFF;
    entry->sel = sel;
    entry->dummy = 0;
    // TODO 用户态取消下面的注释
    entry->flags = flags /* | 0x60 */;
}

void x86::init_idt() {
    // 8259操作
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
    idt_ptr.base = reinterpret_cast<uint32_t>(&idt_table);
    set_idt_entry(0, reinterpret_cast<uint32_t>(isr0), 0x08, 0x8E);
    set_idt_entry(1, reinterpret_cast<uint32_t>(isr1), 0x08, 0x8E);
    set_idt_entry(2, reinterpret_cast<uint32_t>(isr2), 0x08, 0x8E);
    set_idt_entry(3, reinterpret_cast<uint32_t>(isr3), 0x08, 0x8E);
    set_idt_entry(4, reinterpret_cast<uint32_t>(isr4), 0x08, 0x8E);
    set_idt_entry(5, reinterpret_cast<uint32_t>(isr5), 0x08, 0x8E);
    set_idt_entry(6, reinterpret_cast<uint32_t>(isr6), 0x08, 0x8E);
    set_idt_entry(7, reinterpret_cast<uint32_t>(isr7), 0x08, 0x8E);
    set_idt_entry(8, reinterpret_cast<uint32_t>(isr8), 0x08, 0x8E);
    set_idt_entry(9, reinterpret_cast<uint32_t>(isr9), 0x08, 0x8E);
    set_idt_entry(10, reinterpret_cast<uint32_t>(isr10), 0x08, 0x8E);
    set_idt_entry(11, reinterpret_cast<uint32_t>(isr11), 0x08, 0x8E);
    set_idt_entry(12, reinterpret_cast<uint32_t>(isr12), 0x08, 0x8E);
    set_idt_entry(13, reinterpret_cast<uint32_t>(isr13), 0x08, 0x8E);
    set_idt_entry(14, reinterpret_cast<uint32_t>(isr14), 0x08, 0x8E);
    set_idt_entry(15, reinterpret_cast<uint32_t>(isr15), 0x08, 0x8E);
    set_idt_entry(16, reinterpret_cast<uint32_t>(isr16), 0x08, 0x8E);
    set_idt_entry(17, reinterpret_cast<uint32_t>(isr17), 0x08, 0x8E);
    set_idt_entry(18, reinterpret_cast<uint32_t>(isr18), 0x08, 0x8E);
    set_idt_entry(19, reinterpret_cast<uint32_t>(isr19), 0x08, 0x8E);
    set_idt_entry(20, reinterpret_cast<uint32_t>(isr20), 0x08, 0x8E);
    set_idt_entry(21, reinterpret_cast<uint32_t>(isr21), 0x08, 0x8E);
    set_idt_entry(22, reinterpret_cast<uint32_t>(isr22), 0x08, 0x8E);
    set_idt_entry(23, reinterpret_cast<uint32_t>(isr23), 0x08, 0x8E);
    set_idt_entry(24, reinterpret_cast<uint32_t>(isr24), 0x08, 0x8E);
    set_idt_entry(25, reinterpret_cast<uint32_t>(isr25), 0x08, 0x8E);
    set_idt_entry(26, reinterpret_cast<uint32_t>(isr26), 0x08, 0x8E);
    set_idt_entry(27, reinterpret_cast<uint32_t>(isr27), 0x08, 0x8E);
    set_idt_entry(28, reinterpret_cast<uint32_t>(isr28), 0x08, 0x8E);
    set_idt_entry(29, reinterpret_cast<uint32_t>(isr29), 0x08, 0x8E);
    set_idt_entry(30, reinterpret_cast<uint32_t>(isr30), 0x08, 0x8E);
    set_idt_entry(31, reinterpret_cast<uint32_t>(isr31), 0x08, 0x8E);
    set_idt_entry(32, reinterpret_cast<uint32_t>(irq0), 0x08, 0x8E);
    set_idt_entry(33, reinterpret_cast<uint32_t>(irq1), 0x08, 0x8E);
    set_idt_entry(34, reinterpret_cast<uint32_t>(irq2), 0x08, 0x8E);
    set_idt_entry(35, reinterpret_cast<uint32_t>(irq3), 0x08, 0x8E);
    set_idt_entry(36, reinterpret_cast<uint32_t>(irq4), 0x08, 0x8E);
    set_idt_entry(37, reinterpret_cast<uint32_t>(irq5), 0x08, 0x8E);
    set_idt_entry(38, reinterpret_cast<uint32_t>(irq6), 0x08, 0x8E);
    set_idt_entry(39, reinterpret_cast<uint32_t>(irq7), 0x08, 0x8E);
    set_idt_entry(40, reinterpret_cast<uint32_t>(irq8), 0x08, 0x8E);
    set_idt_entry(41, reinterpret_cast<uint32_t>(irq9), 0x08, 0x8E);
    set_idt_entry(42, reinterpret_cast<uint32_t>(irq10), 0x08, 0x8E);
    set_idt_entry(43, reinterpret_cast<uint32_t>(irq11), 0x08, 0x8E);
    set_idt_entry(44, reinterpret_cast<uint32_t>(irq12), 0x08, 0x8E);
    set_idt_entry(45, reinterpret_cast<uint32_t>(irq13), 0x08, 0x8E);
    set_idt_entry(46, reinterpret_cast<uint32_t>(irq14), 0x08, 0x8E);
    set_idt_entry(47, reinterpret_cast<uint32_t>(irq15), 0x08, 0x8E);
    set_idt_entry(0x80, reinterpret_cast<uint32_t>(isr128), 0x08, 0x8E);

    asm volatile("lidt %0" : : "m"(idt_ptr));
}