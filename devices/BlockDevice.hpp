//
// Created by Harold on 2020/5/8.
//

#ifndef OS_CPP_BLOCKDEVICE_HPP
#define OS_CPP_BLOCKDEVICE_HPP

#include <sys/types.hpp>
#include <devices/Device.hpp>

namespace kernel {
    typedef uint32_t BlockIndex;

    class BlockDevice : Device {
    protected:
        size_t block_size_ = 0;
    public:
        size_t block_size() const {
            return block_size_;
        }
//        virtual ssize_t read(uint8_t *buffer, size_t block_num, size_t count) = 0;
//
//        virtual ssize_t write(uint8_t *buffer, size_t block_num, size_t count) = 0;

        virtual ssize_t block_read(size_t from, size_t count, uint8_t *buffer) = 0;
//        virtual ssize_t read_bytes(size_t from, size_t count, uint8_t *buffer) = 0;
    protected:
        explicit BlockDevice(size_t block_size);
    };
}

#endif //OS_CPP_BLOCKDEVICE_HPP
