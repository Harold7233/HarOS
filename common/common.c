#include <common/common.h>
void outb(u16int_t port, u8int_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int_t inb(u16int_t port)
{
   u8int_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}