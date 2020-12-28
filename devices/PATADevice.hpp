//
// Created by Harold on 2020/5/16.
//

#ifndef OS_CPP_PATADEVICE_HPP
#define OS_CPP_PATADEVICE_HPP

#include <sys/types.hpp>

namespace kernel {


    class PATAChannel;
    enum PATADeviceDriveType : uint8_t {
        MASTER = 0x0,
        SLAVE = 0x1
    };
    class PATADevice {
        friend class PATAChannel;
        friend class PATAHardDisk;
        uint8_t exists_{};    // 0 (Empty) or 1 (This Drive really exists).
        uint8_t channel_num{};     // 0 (Primary Channel) or 1 (Secondary Channel).
        PATADeviceDriveType drive_type_{};       // 0 (Master Drive) or 1 (Slave Drive).
        uint16_t type{};        // 0: ATA, 1:ATAPI.
        uint16_t signature{};   // Drive Signature
        uint16_t capabilities{};// Features.
        uint32_t command_sets{}; // Command Sets Supported.
        uint32_t size{};        // Size in Sectors.
        uint8_t model[41]{};   // Model in string.
        size_t sector_size_{512};
        PATAChannel &channel_;

        explicit PATADevice(PATAChannel&);
    public:
        bool exists() const {
            return exists_;
        }

        bool is_master() const {
            return drive_type_ == PATADeviceDriveType::MASTER;
        }

        bool is_slave() const {
            return drive_type_ == PATADeviceDriveType::SLAVE;
        }

        ssize_t read_sectors(uint8_t sector_count, uint32_t lba,
                             uint8_t *buffer);
    };
}
#endif //OS_CPP_PATADEVICE_HPP
