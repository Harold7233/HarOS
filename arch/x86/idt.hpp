#ifndef _IDT_H
#define _IDT_H

#include <sys/types.hpp>
#include <sys/gcc.hpp>

namespace x86 {
    struct PACKED idt_entry_t {
        uint16_t base_lo;
        uint16_t sel;
        uint8_t dummy;
        uint8_t flags;
        uint16_t base_hi;
    };

    struct PACKED idt_ptr_t {
        uint16_t limit;
        uint32_t base;
    };

    void init_idt();
}  // namespace x86

#endif