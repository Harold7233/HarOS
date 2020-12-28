#ifndef _X86_TIMER_H
#define _X86_TIMER_H

#include <sys/asm.hpp>
#include <arch/x86/interrupt.hpp>


namespace x86 {
    constexpr uint32_t FREQUENCY = 100;
    class Timer : IRQHandler {
        Timer();

        void handle_irq(intr_frame_t &) override;

    public:
        static void init();
    };
}  // namespace x86

#endif