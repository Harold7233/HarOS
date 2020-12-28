//
// Created by Harold on 2020/6/1.
//

#include <kernel/tty/TTY.hpp>
#include <libcpp/cstring.hpp>

namespace kernel {


    void TTY::write(const char *str, size_t count) {
        int i = 0;
        size_t min_buffer_pos = out_buf_pos_;
        size_t max_buffer_pos = out_buf_pos_;
        while (count--) {
            char c = str[i++];
            uint8_t cur_column = out_buf_pos_ % cols_;
            // 退格键
            if (c == '\b' && cur_column) {
                // Backspace
                min_buffer_pos = MIN(out_buf_pos_, min_buffer_pos);
                --out_buf_pos_;
            } else if (c == '\t') {
                // FIXME 应该可以移动到下一行
                // tab键，x移动到下一个8的倍数
                auto new_column = MIN((cur_column + 8) & ~0b111, cols_);
                for (int j = 0; j < new_column - cur_column; ++j) {
                    set_and_inc_buffer_pos(0);
                }
                max_buffer_pos = MAX(max_buffer_pos, out_buf_pos_);
            } else if (c == '\r') {
                // 回到行首
                min_buffer_pos = MIN(out_buf_pos_, min_buffer_pos);;
                out_buf_pos_ -= cur_column;
            } else if (c == '\n') {
                // 换行
                size_t new_buffer_pos = (out_buf_pos_ + cols_) / cols_ * cols_;
                size_t old = out_buf_pos_;
                for (size_t j = 0; j < new_buffer_pos - old; ++j) {
//                    set_and_inc_buffer_pos(0);
                    inc_buffer_pos();
                }
                max_buffer_pos = MAX(max_buffer_pos, out_buf_pos_);
            } else if (c >= ' ') {
                uint16_t t = c | ((VGATextColor::FORE_WHITE | VGATextColor::BACK_BLACK) << 8);
                set_and_inc_buffer_pos(t);
                max_buffer_pos = MAX(max_buffer_pos, out_buf_pos_);
            }
        }
        if (max_buffer_pos < window_top_pos_ + screen_size()) {
            // 如果不需要翻页，直接输出新增加的字符串
            print(window_top_pos_, min_buffer_pos, max_buffer_pos - min_buffer_pos);
        } else {
            // 需要翻页
            window_top_pos_ = (max_buffer_pos + cols_) / cols_ * cols_ - screen_size();
            size_t t = max_buffer_pos;
            // 翻页后新行的剩余内容清空
            size_t line_end = window_top_pos_ + screen_size() - 1;
            while (t <= line_end) {
                buffer_[t++ % out_buf_size()] = VGA_BLANK_CHARACTER;
            }
            if (input_mode_) {
                set_cursor_state(true);
            }
            print();
        }
    }

    void TTY::scroll_up(size_t lines) {
        size_t top_limit = 0;
        size_t old = window_top_pos_;
        if (out_buf_pos_ < out_buf_size()) {
            // 还没开始循环，从buffer_[0]开始都有效
            top_limit = 0;
        } else {
            // 还在缓冲区中的最远的下标位置是buffer_pos%out_buf_size()+1
            // 线性下标为buffer_pos + 1 - out_buf_size()
            top_limit = out_buf_pos_ + 1 - out_buf_size();
            // 对齐到行首
            if (top_limit % cols_ != 0) {
                top_limit = (top_limit / cols_ + 1) * cols_;
            }
        }
        if (window_top_pos_ > cols_ * lines) { // 等价于if(window_top_pos_>0)，因为window_top_pos_一定是cols的整数倍
            auto new_window_top = window_top_pos_ - cols_ * lines;
            if (new_window_top < top_limit)
                window_top_pos_ = top_limit;
            else
                window_top_pos_ = new_window_top;
        } else {
            window_top_pos_ = top_limit;
        }
        if (window_top_pos_ != old) {
            print();
            if (window_top_pos_ + screen_size() < out_buf_pos_) {
                set_cursor_state(false);
            }
        }

    }

    void TTY::scroll_down(size_t lines) {
        size_t top_wnd_pos_btm_limit = 0;
        size_t old_window_top_pos = window_top_pos_;
        if (out_buf_pos_ < out_buf_size() - screen_size()) {
            // 当缓冲区空白区域大小至少为一屏时
            // 可以往下滚动的最远位置是当buffer_pos_所在的行位于屏幕最上方时
            top_wnd_pos_btm_limit = out_buf_pos_ - out_buf_pos_ % cols_;
        } else {
            // 否则最远的位置是当buffer_pos_所在的行位于屏幕最下方时
            top_wnd_pos_btm_limit = out_buf_pos_ - out_buf_pos_ % cols_ - (rows_ - 1) * cols_;
        }
        if (window_top_pos_ < 0xFFFFFFFF - cols_ * lines) {
            if (window_top_pos_ + cols_ * lines <= top_wnd_pos_btm_limit) {
                window_top_pos_ += cols_ * lines;
            } else {
                window_top_pos_ = top_wnd_pos_btm_limit;
            }
        } else {
            window_top_pos_ = top_wnd_pos_btm_limit;
        }
        if (window_top_pos_ != old_window_top_pos) {
            print();
            if (window_top_pos_ + screen_size() >= out_buf_pos_) {
                set_cursor_state(input_mode_);
                move_cursor();
            }
        }
    }

    void TTY::clear() {
        for (size_t i = 0; i < out_buf_size(); ++i) {
            buffer_[i] = VGA_BLANK_CHARACTER;
        }
        for (size_t i = 0; i < screen_size(); ++i) {
            vga_memory_[i] = VGA_BLANK_CHARACTER;
        }
        window_top_pos_ = 0;
        out_buf_pos_ = 0;
    }

    void TTY::print(size_t window_start_index, size_t offset_index, size_t length) {
        size_t vga_offset = 0;
        vga_offset = offset_index - window_start_index;
        uint16_t c;
        while (length--) {
            c = buffer_[offset_index++ % out_buf_size()];
            vga_memory_[vga_offset++] = static_cast<uint16_t>(c);
        }
        if (input_mode_) {
            move_cursor();
        }
    }

    void TTY::set_cursor_state(bool enabled) {
        if (cursor_enabled == enabled) {
            return;
        }
        if (enabled) {
            outb(0x3D4, 0x0A);
            outb(0x3D5, (inb(0x3D5) & 0xC0) | CURSOR_START_LINE); // 光标从第14线开始
            outb(0x3D4, 0x0B);
            outb(0x3D5, (inb(0x3D5) & 0xE0) | CURSOR_END_LINE); // 光标到第15线结束
        } else {
            outb(0x3D4, 0x0A);
            outb(0x3D5, 0x20);
        }
        cursor_enabled = enabled;
    }

    void TTY::move_cursor(uint8_t row, uint8_t col) {
        uint16_t new_pos = row * cols_ + col;
        outb(0x3D4, 0x0F);            // 准备更新低8位
        outb(0x3D5, new_pos & 0xFF);  // 发送低8位
        outb(0x3D4, 0x0E);            // 准备更新高8位
        outb(0x3D5, (new_pos >> 8) & 0xFF);       // 发送高8位
    }

    void TTY::move_cursor(size_t buffer_pos) {
//        vga_memory_[buffer_pos - window_top_pos_] = VGA_BLANK_CHARACTER;
        uint16_t new_pos = buffer_pos - window_top_pos_;
        outb(0x3D4, 0x0F);            // 准备更新低8位
        outb(0x3D5, new_pos & 0xFF);  // 发送低8位
        outb(0x3D4, 0x0E);            // 准备更新高8位
        outb(0x3D5, (new_pos >> 8) & 0xFF);       // 发送高8位
    }

    void TTY::on_key_pressed(KeyCombination key) {
        if (!key.has_modifiers()) {

        }
    }

    void TTY::put_to_input_buffer(char c) {
        if (input_buffer_pos_ == TTY_INPUT_BUFFER_SIZE)
            return;
    }

}
