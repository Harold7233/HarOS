//
// Created by Harold on 2020/4/10.
//
#include <devices/vga.hpp>
#include <kernel/stdio.hpp>
#include <libcpp/cstring.hpp>
#include <sys/asm.hpp>
#include <kernel/locks/semaphore.hpp>


namespace {
    void kprintf_color_va_list(uint8_t attribute, const char *fmt, va_list list) {
        char buffer[256];
        vsprintf(buffer, fmt, list);
        devices::vga::print_color(buffer, attribute);
    }
}

namespace kernel {
    void kprint(const char *fmt, ...) {
        va_list list;
        va_start(list, fmt);
        kprintf_color_va_list(
                uint8_t(devices::vga::TextColor::BACK_BLACK) | uint8_t(devices::vga::TextColor::FORE_WHITE), fmt, list);
        va_end(list);

    }

    void kprintf_color(uint8_t attribute, const char *fmt, ...) {
        va_list list;
        va_start(list, fmt);
        kprintf_color_va_list(attribute, fmt, list);
        va_end(list);
    }

    void panic_detail(const char *msg, const char *file, uint32_t line, uint8_t color) {
        asm volatile ("cli");
        devices::vga::__invalidate__lock__();
        devices::vga::clear();
        kprintf_color(color, msg);
        if (file && line) {
            kprintf_color(color, " (File: %s, Line: %d)\n", file, line);
        }
        loop();
    }

    void panic(const char *msg_fmt, ...) {
        asm volatile ("cli");
        devices::vga::__invalidate__lock__();
        va_list l;
        va_start(l, msg_fmt);
        kprintf_color_va_list(uint8_t(devices::vga::TextColor::FORE_RED), msg_fmt, l);
        va_end(l);
    }

    void panic(const char *msg);
}