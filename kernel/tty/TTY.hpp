//
// Created by Harold on 2020/6/1.
//

#ifndef OS_CPP_TTY_HPP
#define OS_CPP_TTY_HPP

#include <kernel/tty/PTYMaster.hpp>
#include <libcpp/CircularQueue.hpp>
#include <libcpp/Vector.hpp>
#include <sys/gcc.hpp>
#include <arch/x86/io.hpp>

#define TTY_INPUT_BUFFER_SIZE 4096
namespace kernel {
    enum VGATextColor {
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
    constexpr uint16_t VGA_BLANK_CHARACTER = (VGATextColor::BACK_BLACK | VGATextColor::FORE_WHITE) << 8 | ' ';

    class TTY {
    private:
        const uint8_t CURSOR_START_LINE = 14;
        const uint8_t CURSOR_END_LINE = 15;

        static TTY *ttys[8];
        uint16_t *const vga_memory_ = reinterpret_cast<uint16_t *>(
                0xB8000 + kernel::KERNEL_VADDR_OFFSET);  //显卡内存从物理地址0xB8000开始
        uint32_t ctrl_flags_{};
        uint8_t cursor_row_{0};
        uint8_t cursor_col_{0};
        uint8_t rows_{25};
        uint8_t cols_{80};
        uint16_t *buffer_;
        size_t out_buf_pos_{0};
        size_t window_top_pos_{0};
        bool wrapped_{false};
        size_t overflow_remainder{0};
        char input_buffer_[TTY_INPUT_BUFFER_SIZE];
        size_t input_buffer_pos_{0};

        void put_to_input_buffer(char c);

        size_t in_buf_pos_{0};
        bool input_mode_{true};
        bool cursor_enabled{false};

        inline size_t out_buf_size() {
            return rows_ * cols_ * 10;
        }

        inline void inc_buffer_pos() {
            ++out_buf_pos_;
            if (unlikely(out_buf_pos_ == 0)) {
                // TODO
            }
        }

        void set_cursor_state(bool enabled);

        void move_cursor(uint8_t row, uint8_t col);

        void move_cursor(size_t buffer_pos);

        inline void move_cursor() {
            if (out_buf_pos_ < window_top_pos_ + screen_size()) {
                move_cursor(out_buf_pos_);
            }
        }


        inline void set_and_inc_buffer_pos(uint16_t c) {
            buffer_[out_buf_pos_ % out_buf_size()] = c;
            inc_buffer_pos();
        }

        void print(size_t window_start_index, size_t offset_index, size_t length);

        inline void print() {
            print(window_top_pos_, window_top_pos_, screen_size());
        }

        void on_key_pressed(KeyCombination key);

    public:
        TTY() {
            buffer_ = new uint16_t[out_buf_size()];

            clear();
            set_cursor_state(true);
        }

        void clear();

        void write(const char *str, size_t count);

        inline void set_input(bool enabled) {
            input_mode_ = enabled;
            if (enabled) {
                set_cursor_state(true);
            } else {
                set_cursor_state(false);
            }
        }

        inline size_t screen_size() {
            return rows_ * cols_;
        }

        inline size_t line_size() {
            return cols_;
        }

        inline void print_char(char character) {
            write(&character, 1);
        }

        void scroll_up(size_t lines = 1);

        void scroll_down(size_t lines = 1);

        inline void scroll_to_bottom() {
            scroll_down(out_buf_size() / cols_);
        }

        inline void scroll_to_top() {
            scroll_up(out_buf_size() / cols_);
        }

    };

}

#endif //OS_CPP_TTY_HPP
