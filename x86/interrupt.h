#include <common/common.h>
#ifndef INT_H
#define INT_H
typedef struct intr_registers
{
    u32int_t ds;                                     // DS选择子
    u32int_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha push的所有寄存器
    u32int_t intr_num, error_code;                   // 中断号和错误号
    u32int_t eip, cs, eflags, useresp, ss;           // 调用ISR之前CPU 自动push的
} intr_regs_t;

typedef void (*intr_handler_cb)(intr_regs_t);

void isr_handler_c(intr_regs_t);
void irq_handler_c(intr_regs_t);
void register_interrupt_handler(u8int_t intr_num, intr_handler_cb handler);
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr128();
#endif