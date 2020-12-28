//
// Created by Harold on 2020/6/10.
//

#ifndef OS_CPP_ATOMIC_HPP
#define OS_CPP_ATOMIC_HPP

#include <sys/types.hpp>
#include <arch/x86/asm.hpp>

namespace kernel {
    template<typename T=int32_t>
    class Atomic {
    private:
        volatile T val;
    public:
        Atomic(T val) : val(val) {

        }

        void add(T value) {
            local_irq_save();
            val += value;
            local_irq_restore();
        }

        void inc() {
            local_irq_save();
            ++val;
            local_irq_restore();
        }


    };
}

#endif //OS_CPP_ATOMIC_HPP
