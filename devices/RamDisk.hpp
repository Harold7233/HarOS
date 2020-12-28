//
// Created by Harold on 2020/5/8.
//

#ifndef OS_CPP_RAMDISK_HPP
#define OS_CPP_RAMDISK_HPP

#include <devices/BlockDevice.hpp>

namespace kernel {
    class RamDisk : public BlockDevice {
        static RamDisk *ins;
        size_t size{};
        uint32_t start{};

        uint32_t addr(uint8_t block_num) {
            return start + block_num * block_size_;
        }

        RamDisk(size_t size, uint32_t start);

    public:
        static void init(size_t size, uint32_t start);

        static RamDisk &instance() {
            return *ins;
        }

//        ssize_t read(uint8_t *buffer, size_t block_num, size_t count) override;
//
//        ssize_t write(uint8_t *buffer, size_t block_num, size_t count) override;
    };
}

#endif //OS_CPP_RAMDISK_HPP
