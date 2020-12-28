//
// Created by Harold on 2020/5/2.
//

#ifndef OS_CPP_KEYBOARD_HPP
#define OS_CPP_KEYBOARD_HPP

#include <sys/types.hpp>
#include <arch/x86/interrupt.hpp>
#include <libcpp/CircularQueue.hpp>
#include <kernel/locks/Spinlock.hpp>
#include <libcpp/assert.hpp>

namespace kernel {
    constexpr uint8_t I8042_BUFFER = 0x60;
    constexpr uint8_t I8042_STATUS = 0x64;
    constexpr uint8_t I8042_ACK = 0xFA;
    constexpr uint8_t I8042_BUFFER_FULL = 0x01;
    constexpr uint8_t I8042_WHICH_BUFFER = 0x20;
    constexpr uint8_t I8042_KEYBOARD_BUFFER = 0x00;
    constexpr uint8_t SHIFT_MASK = 0x1;
    constexpr uint8_t CTRL_MASK = 0x2;
    constexpr uint8_t ALT_MASK = 0x4;
    constexpr uint8_t CMD_MASK = 0x8;

    enum KeyCode : uint8_t {
        KeyError = 0,
        KeyEscape,
        KeyA = 0x1E,
        KeyB = 0x30,
        KeyC = 0x2E,
        KeyD = 0x20,
        KeyE = 0x12,
        KeyF = 0x21,
        KeyG = 0x22,
        KeyH = 0x23,
        KeyI = 0x17,
        KeyJ = 0x24,
        KeyK = 0x25,
        KeyL = 0x26,
        KeyM = 0x32,
        KeyN = 0x31,
        KeyO = 0x18,
        KeyP = 0x19,
        KeyQ = 0x10,
        KeyR = 0x13,
        KeyS = 0x1F,
        KeyT = 0x14,
        KeyU = 0x16,
        KeyV = 0x2F,
        KeyW = 0x11,
        KeyX = 0x2D,
        KeyY = 0x15,
        KeyZ = 0x2C,
        Key0 = 0x0B,
        Key1 = 0x02,
        Key2 = 0x03,
        Key3 = 0x04,
        Key4 = 0x05,
        Key5 = 0x06,
        Key6 = 0x07,
        Key7 = 0x08,
        Key8 = 0x09,
        Key9 = 0x0A,
        KeyBackQuote = 0x29,
        KeyMinus = 0x0C,
        KeyEqual = 0x0D,
        KeyBackSlash = 0x2B,
        KeyBackSpace = 0x0E,
        KeySpace = 0x39,
        KeyTab = 0x0F,
        KeyCAPS = 0x3A,
        KeyLeftShift = 0x2A,
        KeyLCtrl = 0x1D,
        KeyLWin = 0x5B,
        KeyLAlt = 0x38,
        KeyRShift = 0x36,
        KeyRCtrl = 0x1D,
        KeyRWin = 0x5C,
        KeyRAlt = 0x38,
        KeyMenus = 0x5D,
        KeyLeftBracket = 0x1A,
        KeyINSERT = 0x52,
        KeyHOME = 0x47,
        KeyPgUp = 0x49,
        KeyDelete = 0x53,
        KeyEND = 0x4F,
        KeyPgDown = 0x51,
        KeyUp = 0x48,
        KeyLeft = 0x4B,
        KeyDown = 0x50,
        KeyRight = 0x4D,
        KeyNUM = 0x45,
        KeyPadSlash = 0x35,
        KeyPadAsterisk = 0x37,
        KeyPadMinus = 0x4A,
        KeyPadPlus = 0x4E,
        KeyPadEnter = 0x1C,
        KeyPadDot = 0x53,
        KeyPad0 = 0x52,
        KeyPad1 = 0x4F,
        KeyPad2 = 0x50,
        KeyPad3 = 0x51,
        KeyPad4 = 0x4B,
        KeyPad5 = 0x4C,
        KeyPad6 = 0x4D,
        KeyPad7 = 0x47,
        KeyPad8 = 0x48,
        KeyPad9 = 0x49,
        KeyRightBracket = 0x1B,
        KeySemiColon = 0x27,
        KeyApostrophe = 0x28,
        KeyComma = 0x33,
        KeyDot = 0x34,
        KeySlash = 0x35,
        KeyTide = KeyBackQuote,
        KeyExclamation = Key1,
        KeyAt = Key2,
        KeyHash = Key3,
        KeyDollar = Key4,
        KeyPercent = Key5,
        KeyCaret = Key6,
        KeyAmpersand = Key7,
        KeyAsterisk = Key8,
        KeyLParenthesis = Key9,
        KeyRParenthesis = Key0,
        KeyUnderscore = KeyMinus,
        KeyPlus = KeyEqual,
        KeyLeftBrace = KeyLeftBracket,
        KeyRightBrace = KeyRightBracket,
        KeyColon = KeySemiColon,
        KeyDoubleQuote = KeyApostrophe,
        KeyLessThan = KeyComma,
        KeyGreaterThan = KeyDot,
        KeyQuestion = KeySlash,

    };

//    enum class KeyCode : uint8_t {
//        KeyError = 0,
//        KeyEscape = KeyCode::KeyEscape,
//        KeyA = KeyCode::KeyA,
//        KeyB = KeyCode::KeyB,
//        KeyC = KeyCode::KeyC,
//        KeyD = KeyCode::KeyD,
//        KeyE = KeyCode::KeyE,
//        KeyF = KeyCode::KeyF,
//        KeyG = KeyCode::KeyG,
//        KeyH = KeyCode::KeyH,
//        KeyI = KeyCode::KeyI,
//        KeyJ = KeyCode::KeyJ,
//        KeyK = KeyCode::KeyK,
//        KeyL = KeyCode::KeyL,
//        KeyM = KeyCode::KeyM,
//        KeyN = KeyCode::KeyN,
//        KeyO = KeyCode::KeyO,
//        KeyP = KeyCode::KeyP,
//        KeyQ = KeyCode::KeyQ,
//        KeyR = KeyCode::KeyR,
//        KeyS = KeyCode::KeyS,
//        KeyT = KeyCode::KeyT,
//        KeyU = KeyCode::KeyU,
//        KeyV = KeyCode::KeyV,
//        KeyW = KeyCode::KeyW,
//        KeyX = KeyCode::KeyX,
//        KeyY = KeyCode::KeyY,
//        KeyZ = KeyCode::KeyZ,
//        Key0 = KeyCode::Key0,
//        Key1 = KeyCode::Key1,
//        Key2 = KeyCode::Key2,
//        Key3 = KeyCode::Key3,
//        Key4 = KeyCode::Key4,
//        Key5 = KeyCode::Key5,
//        Key6 = KeyCode::Key6,
//        Key7 = KeyCode::Key7,
//        Key8 = KeyCode::Key8,
//        Key9 = KeyCode::Key9,
//        KeyBackQuote = KeyCode::KeyBackQuote,
//        KeyMinus = KeyCode::KeyMinus,
//        KeyEqual = KeyCode::KeyEqual,
//        KeyBackSlash = KeyCode::KeyBackSlash,
//        KeyBackSpace = KeyCode::KeyBackSpace,
//        KeySpace = KeyCode::KeySpace,
//        KeyTab = KeyCode::KeyTab,
//        KeyCAPS = KeyCode::KeyCAPS,
//        KeyLeftShift = KeyCode::KeyLeftShift,
//        KeyLCtrl = KeyCode::KeyLCtrl,
//        KeyLWin = KeyCode::KeyLWin,
//        KeyLAlt = KeyCode::KeyLAlt,
//        KeyRShift = KeyCode::KeyRShift,
//        KeyRCtrl = KeyCode::KeyRCtrl,
//        KeyRWin = KeyCode::KeyRWin,
//        KeyRAlt = KeyCode::KeyRAlt,
//        KeyMenus = KeyCode::KeyMenus,
//        KeyLeftBracket = KeyCode::KeyLeftBracket,
//        KeyINSERT = KeyCode::KeyINSERT,
//        KeyHOME = KeyCode::KeyHOME,
//        KeyPgUp = KeyCode::KeyPgUp,
//        KeyDelete = KeyCode::KeyDelete,
//        KeyEND = KeyCode::KeyEND,
//        KeyPgDown = KeyCode::KeyPgDown,
//        KeyUp = KeyCode::KeyUp,
//        KeyLeft = KeyCode::KeyLeft,
//        KeyDown = KeyCode::KeyDown,
//        KeyRight = KeyCode::KeyRight,
//        KeyNUM = KeyCode::KeyNUM,
//        KeyPadSlash = KeyCode::KeyPadSlash,
//        KeyPadAsterisk = KeyCode::KeyPadAsterisk,
//        KeyPadMinus = KeyCode::KeyPadMinus,
//        KeyPadPlus = KeyCode::KeyPadPlus,
//        KeyPadEnter = KeyCode::KeyPadEnter,
//        KeyPadDot = KeyCode::KeyPadDot,
//        KeyPad0 = KeyCode::KeyPad0,
//        KeyPad1 = KeyCode::KeyPad1,
//        KeyPad2 = KeyCode::KeyPad2,
//        KeyPad3 = KeyCode::KeyPad3,
//        KeyPad4 = KeyCode::KeyPad4,
//        KeyPad5 = KeyCode::KeyPad5,
//        KeyPad6 = KeyCode::KeyPad6,
//        KeyPad7 = KeyCode::KeyPad7,
//        KeyPad8 = KeyCode::KeyPad8,
//        KeyPad9 = KeyCode::KeyPad9,
//        KeyRightBracket = KeyCode::KeyRightBracket,
//        KeySemiColon = KeyCode::KeySemiColon,
//        KeyApostrophe = KeyCode::KeyApostrophe,
//        KeyComma = KeyCode::KeyComma,
//        KeyDot = KeyCode::KeyDot,
//        KeySlash = KeyCode::KeySlash,
////        KeyTide = KeyBackQuote | 0x80,
////        KeyExclamation = Key1 | 0x80,
////        KeyAt = Key2 | 0x80,
////        KeyHash = Key3 | 0x80,
////        KeyDollar = Key4 | 0x80,
////        KeyPercent = Key5 | 0x80,
////        KeyCaret = Key6 | 0x80,
////        KeyAmpersand = Key7 | 0x80,
////        KeyAsterisk = Key8 | 0x80,
////        KeyLParenthesis = Key9 | 0x80,
////        KeyRParenthesis = Key0 | 0x80,
////        KeyUnderscore = KeyMinus | 0x80,
////        KeyPlus = KeyEqual | 0x80,
////        KeyLeftBrace = KeyLeftBracket | 0x80,
////        KeyRightBrace = KeyRightBracket | 0x80,
////        KeyColon = KeySemiColon | 0x80,
////        KeyDoubleQuote = KeyApostrophe | 0x80,
////        KeyLessThan = KeyComma | 0x80,
////        KeyGreaterThan = KeyDot | 0x80,
////        KeyQuestion = KeySlash | 0x80,
//    };

    class KeyCombination {
    private:
        uint8_t modifiers{0};
    public:
        KeyCode key{KeyCode::KeyError};
        char ascii_char{0};
        bool make{true};

        bool has_shift() {
            return modifiers & SHIFT_MASK;
        }

        bool has_ctrl() {
            return modifiers & CTRL_MASK;
        }

        bool has_alt() {
            return modifiers & ALT_MASK;
        }

        bool has_cmd() {
            return modifiers & CMD_MASK;
        }

        bool has_only_ctrl() {
            return modifiers == CTRL_MASK;
        }

        bool has_modifiers() {
            return modifiers != 0;
        }
    };

    class Keyboard : public x86::IRQHandler {
        static Keyboard *ins_;
        enum class KeyCodeState : uint8_t {
            standby = 0, e0, e0_modifiers, modifiers
        };
        uint8_t modifiers_;
        KeyCodeState status{KeyCodeState::standby};

        void handle_irq(x86::intr_frame_t &frame) override;

        Keyboard() : IRQHandler(33) {

        }

        CircularQueue<KeyCombination> queue_{100};
        kernel::Spinlock spinlock_;

    public:
        inline static Keyboard &instance() {
            return *ins_;
        }

        static void init() {
            ins_ = new Keyboard();
        }

        KeyCombination get_key() {
            while (true) {
                while (queue_.empty());
                spinlock_.acquire();
                if (queue_.empty()) {
                    continue;
                }
                auto key = queue_.dequeue();
                spinlock_.release();
                return key;
            }
        }

        size_t get_keys(KeyCombination *buffer, size_t max_count) {
            while (true) {
                while (queue_.empty());
                spinlock_.acquire();
                if (queue_.empty()) {
                    continue;
                }
                size_t i = 0;
                while (!queue_.empty() && i < max_count) {
                    buffer[i++] = queue_.dequeue();
                }
                spinlock_.release();
                return i;
            }
        }

    };


}

#endif //OS_CPP_KEYBOARD_HPP
