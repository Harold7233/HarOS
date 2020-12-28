#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

#include <devices/vga.hpp>

#define PANIC(msg) panic_detail(msg, __FILE__, __LINE__, uint8_t(devices::vga::TextColor::FORE_RED))

namespace kernel {
    void kprint(const char *fmt, ...);

    void kprintf_color(uint8_t attr, const char *fmt, ...);

    void panic_detail(const char *msg, const char *file, uint32_t line,
                      uint8_t color);

    void panic(const char *msg_fmt, ...);

    void panic(const char *msg);

}  // namespace kernel
using namespace kernel;
#endif