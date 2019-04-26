#include <common/common.h>
#ifndef IDT_H
#define IDT_H
typedef struct idt_entry_struct
{
    u16int_t base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
    u16int_t sel;     // Kernel segment selector.
    u8int_t dummy;    // This must always be zero.
    u8int_t flags;    // More flags. See documentation.
    u16int_t base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed)) idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct idt_reg_struct
{
    u16int_t limit;
    u32int_t base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed)) idt_ptr_t;
  
extern void load_idt(u32int_t);
void init_idt();

#endif