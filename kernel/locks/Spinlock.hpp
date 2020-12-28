//
// Created by Harold on 2020/6/12.
//

#ifndef OS_CPP_SPINLOCK_HPP
#define OS_CPP_SPINLOCK_HPP

#include <arch/x86/asm.hpp>

namespace kernel {
    class Spinlock {
    private:
        uint32_t irq_state;
    public:
        inline void acquire() {
            save_irq_and_disable(irq_state);
        }

        inline void release() {
            restore_irq(irq_state);
        }
    };


}


#endif //OS_CPP_SPINLOCK_HPP
