#include <arch/x86/timer.hpp>
#include <devices/vga.hpp>
#include <arch/x86/interrupt.hpp>
#include <kernel/tasking/Scheduler.hpp>
#include <arch/x86/io.hpp>
#define IRQ_TIMER 32
namespace {
    x86::Timer *timer{};
}  // namespace


namespace x86 {
    void Timer::init() {
        constexpr uint32_t divisor = 1193180 / FREQUENCY;

        outb(0x43, 0x36);

        constexpr auto low = static_cast<uint8_t>(divisor & 0xFF);
        constexpr auto high = static_cast<uint8_t>((divisor >> 8) & 0xFF);

        outb(0x40, low);
        outb(0x40, high);
        timer = new Timer;
    }

    void Timer::handle_irq(intr_frame_t &) {
        scheduler.tick();
    }

    Timer::Timer() : IRQHandler(IRQ_TIMER) {

    }

}
