//
// Created by Harold on 2020/6/14.
//

#ifndef OS_CPP_VGADEVICE_HPP
#define OS_CPP_VGADEVICE_HPP

#include <sys/types.hpp>
#include <arch/x86/io.hpp>
#include <devices/BlockDevice.hpp>
#include <kernel/memory/MemoryManager.hpp>

namespace kernel {
//    enum TextColor {
//        FORE_BLACK = 0x0,
//        FORE_BLUE = 0x1,
//        FORE_GREEN = 0x2,
//        FORE_CYAN = 0x3,
//        FORE_RED = 0x4,
//        FORE_MAGENTA = 0x5,
//        FORE_BROWN = 0x6,
//        FORE_GRAY = 0x7,
//        FORE_WHITE = 0xF,
//        BACK_BLACK = 0x0,
//        BACK_BLUE = 0x1 << 4,
//        BACK_GREEN = 0x2 << 4,
//        BACK_CYAN = 0x3 << 4,
//        BACK_RED = 0x4 << 4,
//        BACK_MAGENTA = 0x5 << 4,
//        BACK_BROWN = 0x6 << 4,
//        BACK_GRAY = 0xF << 4,
//        BLINK = 0xF0
//    };
//
//    class VGADevice {
//    private:
//        uint8_t cursor_row_{0};
//        uint8_t cursor_col_{0};
//        uint8_t rows_{25};
//        uint8_t cols_{80};
//        uint16_t *const video_memory = reinterpret_cast<uint16_t *>(
//                0xB8000 + kernel::KERNEL_VADDR_OFFSET);  //显卡内存从物理地址0xB8000开始
//    public:
//
//        void toggle_cursor(bool enable) {
//            if (enable) {
//                outb(0x3D4, 0x0A);
//                outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
//                outb(0x3D4, 0x0B);
//                outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
//            } else {
//                outb(0x3D4, 0x0A);
//                outb(0x3D5, 0x20);
//            }
//        }
//
//        uint16_t get_cursor_pos() {
//            uint16_t pos = 0;
//            outb(0x3D4, 0x0F);
//            pos |= inb(0x3D5);
//            outb(0x3D4, 0x0E);
//            pos |= ((uint16_t) inb(0x3D5)) << 8;
//            return pos;
//        }
//
//        void update_cursor() const {
//            uint16_t new_pos = cursor_row_ * cols_ + cursor_col_;
//            outb(0x3D4, 14);            // IO操作，准备更新高8位
//            outb(0x3D5, new_pos >> 8);  // 发送高8位
//            outb(0x3D4, 15);            // 准备更新低8位
//            outb(0x3D5, new_pos);       // 发送低8位
//        }
//
//        /* 屏幕上翻 */
//        void scroll() {
//            // 0-24行，如果位于25行说明需要上翻
//            if (cursor_row_ >= rows_) {
//                //当前屏幕的所有文字全部上移一行，即buffer的内容全部往前移动80个字节，第一行的扔掉
//                for (int i = 0; i < (rows_ - 1) * cols_; ++i) {
//                    video_memory[i] = video_memory[i + cols_];
//                }
//
//                // 最后一行是空行，并且光标位于此行第一行
//                for (int i = (rows_ - 1) * cols_; i < rows_ * cols_;
//                     i++) {
//                    video_memory[i] = ' ' | (TextColor::BACK_BLACK | TextColor::FORE_WHITE) << 4;
//                }
//                cursor_row_ = rows_ - 1;
//            }
//        }
//
//
//    };
}


#endif //OS_CPP_VGADEVICE_HPP
