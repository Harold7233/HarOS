#include <x86/interrupt.h>
#include <common/dev.h>
#include <task/sched.h>
intr_handler_t interrupt_handlers[256];
void isr_handler_c(intr_regs_t registers)
{
    if (interrupt_handlers[registers.intr_num])
    {
        intr_handler_t handler = interrupt_handlers[registers.intr_num];
        handler(registers);
    }

    if (current_task && current_task->need_resched && !current_task->preempt_count)
    {
        schedule();
    }
}

void irq_handler_c(intr_regs_t registers)
{
    if (registers.intr_num >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
    if (interrupt_handlers[registers.intr_num])
    {
        intr_handler_t handler = interrupt_handlers[registers.intr_num];
        handler(registers);
    }
    if (current_task && current_task->need_resched && !current_task->preempt_count)
    {
        schedule();
    }
}

void register_interrupt_handler(u8int_t intr_num, intr_handler_t handler)
{
    interrupt_handlers[intr_num] = handler;
}