//
// Created by Harold on 2020/5/17.
//

#include <devices/PATADevice.hpp>
#include <devices/PATAChannel.hpp>

namespace kernel {
    PATADevice::PATADevice(PATAChannel &channel) : channel_(channel) {

    }

    ssize_t PATADevice::read_sectors(uint8_t sector_count, uint32_t lba, uint8_t *buffer) {
        return channel_.read_sectors(drive_type_, sector_count, lba, buffer);
    }
}