//
// Created by Harold on 2020/5/16.
//

#ifndef OS_CPP_PATAHARDDISK_HPP
#define OS_CPP_PATAHARDDISK_HPP

#include <devices/PATADevice.hpp>
#include <devices/BlockDevice.hpp>

namespace kernel {
    class PATAHardDisk : public BlockDevice {
    private:
        PATADevice &device_;
        size_t sectors_per_block_{1};
    public:
        PATAHardDisk(size_t blockSize, PATADevice &pata_device) : BlockDevice(pata_device.sector_size_),
                                                                  device_(pata_device) {

        }

        ssize_t block_read(size_t from, size_t count, uint8_t *buffer) override;

//        ssize_t read_bytes(size_t from, size_t count, uint8_t *buffer) override;
    };
}
#endif //OS_CPP_PATAHARDDISK_HPP
