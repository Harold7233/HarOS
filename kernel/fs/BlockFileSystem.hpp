//
// Created by Harold on 2020/5/17.
//

#ifndef OS_CPP_BLOCKFILESYSTEM_HPP
#define OS_CPP_BLOCKFILESYSTEM_HPP

#include <kernel/fs/FileSystem.hpp>
#include <devices/BlockDevice.hpp>

namespace kernel {

    class BlockFileSystem : public FileSystem {
    protected:
        BlockDevice &device_;
        size_t block_size_{};

        explicit BlockFileSystem(BlockDevice &device) : device_(device) {

        }

    public:
        ssize_t block_read(BlockIndex from, size_t count, uint8_t *buffer) {
            size_t total_bytes = from * block_size_;
            size_t count_bytes = count * block_size_;
            return device_.block_read(total_bytes / device_.block_size(), count_bytes / device_.block_size(), buffer);
        }

    };

}

#endif //OS_CPP_BLOCKFILESYSTEM_HPP
