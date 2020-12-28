#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <sys/types.hpp>
#include <sys/gcc.hpp>

namespace x86 {
    constexpr uint8_t IRQ0 = 32;
    constexpr uint8_t IRQ1 = 33;
    constexpr uint8_t IRQ2 = 34;
    constexpr uint8_t IRQ3 = 35;
    constexpr uint8_t IRQ4 = 36;
    constexpr uint8_t IRQ5 = 37;
    constexpr uint8_t IRQ6 = 38;
    constexpr uint8_t IRQ7 = 39;
    constexpr uint8_t IRQ8 = 40;
    constexpr uint8_t IRQ9 = 41;
    constexpr uint8_t FAULT_PAGE = 14;
    constexpr uint8_t PF_PROTECTION_NON_PRESENT = 0x1;
    constexpr uint8_t PF_WRITE_READ = 0x2;
    constexpr uint8_t PF_USER_KERNEL = 0x4;
    struct PACKED intr_frame_t {
        uint16_t ds{};                                      // DS选择子
        uint32_t edi{}, esi{}, ebp{}, dummy_esp{}, ebx{}, edx{}, ecx{}, eax{};  // pusha push的所有寄存器
        uint32_t intr_or_irq{}, error_code{};                    // 中断号和错误号
        struct PACKED original_intr_frame_t {
            uint32_t eip{}, cs{}, eflags{};
            uint32_t user_esp{}, ss{};
        } context;
    };

    typedef void (*intr_handler_t)(intr_frame_t);

    class IRQHandler {
        friend void intr_handler_cpp_linkage(intr_frame_t &);

    protected:
        explicit IRQHandler(uint8_t irq_num);

    public:
        virtual void handle_irq(intr_frame_t &frame) = 0;
    };


}  // namespace x86
extern "C" {
void intr_handler(x86::intr_frame_t);
void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();
void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();
void isr128();
}
#endif