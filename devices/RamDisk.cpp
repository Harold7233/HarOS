//
// Created by Harold on 2020/5/8.
//

#include <devices/RamDisk.hpp>
#include <kernel/memory/MemoryManager.hpp>
#include <libcpp/cstring.hpp>
#include <libcpp/cmath.hpp>

namespace kernel {
    RamDisk *RamDisk::ins = nullptr;

    RamDisk::RamDisk(size_t size, uint32_t start) : BlockDevice(PAGE_SIZE), start(start) {
        auto count = ceil_divide(size, block_size_);
    }


}