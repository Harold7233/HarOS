//
// Created by Harold on 2020/5/12.
//

#ifndef OS_CPP_SIMPLEVGA_HPP
#define OS_CPP_SIMPLEVGA_HPP

#include <devices/BlockDevice.hpp>

namespace kernel {
    constexpr uint16_t VGA_WIDTH = 80;
    constexpr uint16_t VGA_HEIGHT = 25;
    enum TextColor : uint8_t {
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

    class SimpleVGA : BlockDevice {

    private:
        TextColor cur_color;

        void scroll() {

        }

    public:
        static void init() {

        }

        void set_color(TextColor color = static_cast<TextColor>(TextColor::FORE_WHITE | TextColor::BACK_BLACK)) {
            cur_color = color;
        }

        void clear();


    };

}

#endif //OS_CPP_SIMPLEVGA_HPP
