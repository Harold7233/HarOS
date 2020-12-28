//
// Created by Harold on 2020/5/13.
//

#include <kernel/tasking/IdleProcess.hpp>
#include <sys/asm.hpp>
#include <devices/RamDisk.hpp>
#include <devices/PATAChannel.hpp>
#include <devices/PATAHardDisk.hpp>
#include <kernel/fs/ext2/Ext2FS.hpp>
#include <devices/Keyboard.hpp>
#include <devices/VGADevice.hpp>
#include <kernel/tty/TTY.hpp>
#include <libcpp/cstring.hpp>

namespace kernel {
    [[noreturn]] int IdleProcess::run(void *args) {
//        devices::vga::clear();
//        PATAChannel::init();
//        auto &p = PATAChannel::secondary();
//        PATAHardDisk hd(512, p.master_device());
//        Ext2FS ext2(hd);
//        auto *root = ext2.get_root_node();
//        ext2.traverse_dir(root);
        devices::vga::clear();
        Keyboard::init();
        TTY a;
        char *all_a = new char[999];
        char *all_b = new char[999];
        memset(all_a, 'a', 999);
        memset(all_b, 'b', 999);
        auto &kb = Keyboard::instance();
        auto *key_buffer = new KeyCombination[100];
        while (true) {
            auto count = kb.get_keys(key_buffer, 100);
            for (int i = 0; i < count; ++i) {
                auto key = key_buffer[i];
                if (key.key == KeyCode::KeyUp) {
                    a.scroll_up();
                } else if (key.key == KeyCode::KeyDown) {
                    a.scroll_down();
                } else {
                    a.write(&key.ascii_char, 1);
                }
            }
        }
        loop();
    }
}
