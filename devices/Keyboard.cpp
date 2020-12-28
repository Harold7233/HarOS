//
// Created by Harold on 2020/5/2.
//

#include <devices/Keyboard.hpp>
#include <sys/asm.hpp>
#include <arch/x86/io.hpp>
#include <kernel/stdio.hpp>

#define MODIFIER 0x1
#define CAN_ESCAPE 0x2
#define CAN_SHIFT = 0x4
#define PRINTABLE = 0x8
#define FN_KEY 0x10


namespace {
    using namespace devices;
    constexpr uint8_t key_type_map[] = {
            0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 2, 3, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 12, 12, 12, 1, 12, 8, 8, 8, 8, 8, 8, 8, 12, 12, 14, 1, 0, 3, 8, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0
    };

    constexpr char ascii_map[] = {
            0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08, '\t', 'q', 'w', 'e', 'r', 't',
            'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
            '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
    };


    constexpr char shift_ascii_map[] = {
            0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x08, '\t',
            'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
            'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
            'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, '|', 0, 0, 0,
    };

    inline bool is_modifier(KeyCode code) {
        return key_type_map[static_cast<uint8_t>(code)] & MODIFIER;
    }

    inline bool can_escape(KeyCode code) {
        return key_type_map[static_cast<uint8_t>(code)] & CAN_ESCAPE;
    }

}

namespace kernel {
    Keyboard *Keyboard::ins_ = nullptr;

    void Keyboard::handle_irq(x86::intr_frame_t &frame) {
        uint8_t i8042_status = inb(I8042_STATUS);
        if (!(((i8042_status & I8042_WHICH_BUFFER) == I8042_KEYBOARD_BUFFER) && (i8042_status & I8042_BUFFER_FULL)))
            return;
        auto data = inb(I8042_BUFFER);
        if (data == 0xe0) {
            return; // TODO e0 prefix
        }
        bool make;
        KeyCode key;
        if (data & 0x80) {
            make = false;
            key = KeyCode(data & 0x7f);
        } else {
            make = true;
            key = KeyCode(data);
        }
        if (is_modifier(key)) {
            uint8_t mask;
            if (key == KeyLeftShift || key == KeyRShift)
                mask = SHIFT_MASK;
            else if (key == KeyLAlt || key == KeyRAlt)
                mask = ALT_MASK;
            else if (key == KeyLCtrl || key == KeyRCtrl) {
                mask = CTRL_MASK;
            } else {
                mask = CMD_MASK;
            }
            if (make)
                modifiers_ |= mask;
            else
                modifiers_ &= ~mask;
        } else {
            if (!make)
                return;
            KeyCombination k;
            k.key = key;
            k.make = make;
            if (modifiers_ == SHIFT_MASK) {
                k.ascii_char = shift_ascii_map[key];
            } else if (modifiers_ == 0) {
                k.ascii_char = ascii_map[key];
            }
            queue_.enqueue(k);
        }
    }


}
