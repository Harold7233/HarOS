#include <x86/timer.h>
#include <x86/interrupt.h>

#include <common/dev.h>
#include <task/sched.h>
u32int_t volatile ticks = 0;

static void timer_callback(intr_regs_t registers)
{
   ticks++;
   scheduler_tick();
}

void init_timer()
{

   register_interrupt_handler(32, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int_t divisor = 1193180 / HZ;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int_t l = (u8int_t)(divisor & 0xFF);
   u8int_t h = (u8int_t)((divisor >> 8) & 0xFF);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);

   asm volatile("sti");
}