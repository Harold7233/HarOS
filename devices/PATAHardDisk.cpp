//
// Created by Harold on 2020/5/16.
//

#include <devices/PATAHardDisk.hpp>

namespace kernel {

    ssize_t PATAHardDisk::block_read(size_t from, size_t count, uint8_t *buffer) {
        return device_.read_sectors(sectors_per_block_ * count, from * sectors_per_block_, buffer);
    }

}