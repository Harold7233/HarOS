//
// Created by Harold on 2020/5/8.
//

#ifndef OS_CPP_ASM_HPP
#define OS_CPP_ASM_HPP

#include <sys/types.hpp>

inline void disable_irq() {
    asm volatile ("cli");
}

inline void enable_irq() {
    asm volatile ("sti");
}

inline void save_irq_and_disable(uint32_t &intr_state) {
    asm volatile ("pushf\n"
                  "movl (%%esp), %0\n"
                  "cli\n"
                  "add $4, %%esp"
    :"=r"(intr_state));
}

inline void restore_irq(uint32_t &intr_state) {
    asm volatile ("test $0x200, %0\n"
                  "jz exit\n"
                  "sti\n"
                  "exit:"
    ::"r"(intr_state));
}

#define local_irq_save() uint32_t _irq_state_;save_irq_and_disable(_irq_state_)
#define local_irq_restore() restore_irq(_irq_state_)
#endif //OS_CPP_ASM_HPP
