#include <devices/vga.hpp>
#include <arch/x86/io.hpp>
#include <sys/asm.hpp>
#include <kernel/kernel.hpp>
#include <kernel/locks/semaphore.hpp>
#include <sys/asm.hpp>
#include <kernel/locks/Mutex.hpp>
// spinlock_t cursor_lock;
namespace {
    volatile uint8_t cursor_x = 0;  //当前光标的位置x
    volatile uint8_t cursor_y = 0;  //当前光标的位置y
    void update_cursor() {
        uint16_t new_pos = cursor_y * devices::vga::VGA_WIDTH + cursor_x;
        outb(0x3D4, 14);            // IO操作，准备更新高8位
        outb(0x3D5, new_pos >> 8);  // 发送高8位
        outb(0x3D4, 15);            // 准备更新低8位
        outb(0x3D5, new_pos);       // 发送低8位
    }

    uint16_t *const video_memory = reinterpret_cast<uint16_t *>(
            0xB8000 + kernel::KERNEL_VADDR_OFFSET);  //显卡内存从物理地址0xB8000开始
}  // namespace
namespace devices {
    namespace vga {
        Mutex *cursor_lock;

        void init() {
            cursor_lock = new Mutex();
        }

        void __invalidate__lock__() {
            cursor_lock = new Mutex();
        }

        /* 屏幕上翻 */
        void scroll() {
            // 0-24行，如果位于25行说明需要上翻
            if (cursor_y >= VGA_HEIGHT) {
                //当前屏幕的所有文字全部上移一行，即buffer的内容全部往前移动80个字节，第一行的扔掉
                for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; ++i) {
                    video_memory[i] = video_memory[i + VGA_WIDTH];
                }
                // 最后一行是空行，并且光标位于此行第一行
                for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH;
                     i++) {
                    video_memory[i] = ' ' | uint8_t(devices::vga::TextColor::BACK_BLACK) |
                                      uint8_t(devices::vga::TextColor::FORE_WHITE) << 4;
                }
                cursor_y = VGA_HEIGHT - 1;
            }
        }

        /*输出一个字符*/
        void putchar_color(char c, uint8_t attribute) {
            char buff[2];
            buff[0] = c;
            buff[1] = '\0';
            print_color(buff, attribute);
        }

        /* 清屏 */
        void clear() {
            if (cursor_lock) cursor_lock->acquire();
            for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
                video_memory[i] = 0xF << 4 | ' ';
            }
            cursor_x = 0;
            cursor_y = 0;
            update_cursor();
            if (cursor_lock) cursor_lock->release();
        }

        /* 输出一个字符串 */
        void print_color(const char *str, uint8_t attribute) {
//            cursor_lock->acquire();
            int i = 0;
            char c = 0;
            while ((c = str[i++])) {
                uint16_t *location = nullptr;
                // 退格键
                if (c == 0x08 && cursor_x) {
                    --cursor_x;
                }
                    // tab键，x移动到下一个8的倍数
                else if (c == 0x09) {
                    cursor_x = (cursor_x + 8) & ~(8 - 1);
                } else if (c == '\r') {
                    cursor_x = 0;
                } else if (c == '\n') {
                    cursor_x = 0;
                    ++cursor_y;
                } else if (c >= ' ') {
                    location = video_memory + (cursor_y * VGA_WIDTH + cursor_x);
                    *location = c | (attribute << 8);
                    ++cursor_x;
                }

                if (cursor_x >= VGA_WIDTH) {
                    cursor_x = 0;
                    ++cursor_y;
                }
            }
            scroll();
            update_cursor();
//            cursor_lock->release();
        }

    }
}
