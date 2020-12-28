#ifndef DEVICES_VGA_H
#define DEVICES_VGA_H

#include <kernel/kernel.hpp>

namespace devices {
    namespace vga {
        constexpr uint16_t VGA_WIDTH = 80;
        constexpr uint16_t VGA_HEIGHT = 25;
        enum class TextColor {
            FORE_BLACK = 0x0,
            FORE_BLUE = 0x1,
            FORE_GREEN = 0x2,
            FORE_CYAN = 0x3,
            FORE_RED = 0x4,
            FORE_MAGENT = 0x5,
            FORE_BROWN = 0x6,
            FORE_GRAY = 0x7,
            FORE_WHITE = 0xF,
            BACK_BLACK = 0x0,
            BACK_BLUE = 0x1 << 4,
            BACK_GREEN = 0x2 << 4,
            BACK_CYAN = 0x3 << 4,
            BACK_RED = 0x4 << 4,
            BACK_MAGENTA = 0x5 << 4,
            BACK_BROWN = 0x6 << 4,
            BACK_GRAY = 0xF << 4,
            BLINK = 0xF0
        };

        void scroll();

        void clear();

        void putchar_color(char, uint8_t);

        void print_color(const char *, uint8_t);

        void hex_option(uint32_t, uint8_t);

        void dec(uint32_t);

        void init();

        void __invalidate__lock__();

//        namespace kernel {
//            class Mutex;
//        }
//        extern kernel::Mutex *cursor_lock;
    }
}  // namespace devices

// #define TEXT_FORE_COLOR_BLUE_LIGHT 0x8
// #define TEXT_FORE_COLOR_GREEN_LIGHT 0x9
// #define TEXT_FORE_COLOR_CYAN_LIGHT 0xA
// #define TEXT_FORE_COLOR_RED_LIGHT 0xB
// #define TEXT_FORE_COLOR_MAGENTA_LIGHT 0xC
// #define TEXT_FORE_COLOR_BLACK_LIGHT 0xD
// #define TEXT_FORE_COLOR_BLACK_LIGHT 0xE
#endif