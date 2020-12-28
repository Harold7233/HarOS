#include <arch/x86/interrupt.hpp>

#include <sys/asm.hpp>
#include <devices/vga.hpp>
#include <kernel/tasking/Scheduler.hpp>
#include <kernel/tasking/Task.hpp>
#include <arch/x86/io.hpp>

namespace {
    x86::IRQHandler *interrupt_handlers[256];
}


namespace x86 {
    IRQHandler::IRQHandler(uint8_t irq_num) {
        interrupt_handlers[irq_num] = this;
    }

    void intr_handler_cpp_linkage(intr_frame_t &frame) {
        uint32_t cr0, cr1, cr2, cr3, cr4;
        uint32_t dr0, dr1, dr2, dr3, dr4, dr5, dr6;
//        if (frame.intr_or_irq == 1) {
        asm volatile ("mov %%dr6, %0":"=r"(dr6));
        asm volatile ("mov %%cr3, %0":"=r"(cr3));
//        }
        if (frame.intr_or_irq >= 40) {
            // 从片重置
            outb(0xA0, 0x20);
        }
//        if (frame.intr_or_irq != 32) {
//            int a = 1;
//            ++ a;
//        }


        // 主片重置
        outb(0x20, 0x20);

        auto *handler = interrupt_handlers[frame.intr_or_irq];
        if (handler) {
            handler->handle_irq(frame);
        }
        if (cur_task.need_reschedule) {
            scheduler.schedule();
        }
    }
}

void intr_handler(x86::intr_frame_t registers) {
    x86::intr_handler_cpp_linkage(registers);
}


