//
// Created by Harold on 2020/5/16.
//
/* From https://wiki.osdev.org/PCI_IDE_Controller */
#ifndef OS_CPP_PATACHANNEL_HPP
#define OS_CPP_PATACHANNEL_HPP

#include <devices/PATADevice.hpp>
// Status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error
// Errors
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark
#define ATA_ERR_DEVICE_NOT_FOUND -1
#define ATA_ERR_INVALID_SEEK -2
//Commands
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC
#define ATAPI_CMD_READ       0xA8
#define ATAPI_CMD_EJECT      0x1B
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200
#define ATA        0x00
#define ATAPI      0x01
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D
#define IDE_ATA        0x00
#define IDE_ATAPI      0x01
#define IDE_UNKNOWN    0x03
// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

namespace kernel {
    enum class PATAChannelType : uint8_t {
        PRIMARY = 0x00,
        SECONDARY = 0x01
    };


    class PATADevice;

    class PATAChannel {
        static PATAChannel *ins_primary, *ins_secondary;
        PATAChannelType type{};
        uint16_t io_base{};
        uint16_t control_base{};
        uint16_t bus_master_ide{};
        uint8_t no_intr{};
        PATADevice *devices[2]{};
        bool irq_invoked{false};
        uint8_t id_buffer[512];

        uint8_t read_reg(uint8_t reg);

        void write_reg(uint8_t reg, uint8_t data);

        void read_reg_data(uint8_t reg, uint8_t *buffer,
                           uint32_t dw_count);

        ssize_t ata_access(bool write, bool dma, PATADeviceDriveType type, uint32_t lba,
                       uint8_t sector_num, uint8_t *buffer);

        uint8_t polling(bool advanced_check);

        uint8_t print_error(PATAChannelType type, uint8_t err);


        explicit PATAChannel(PATAChannelType, uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3,
                             uint32_t BAR4);

    public:
        static void init();

        ssize_t read_sectors(PATADeviceDriveType type, uint8_t sector_count, uint32_t lba,
                             uint8_t *buffer);

        PATADevice &master_device() {
            return *devices[0];
        }

        PATADevice &slave_device() {
            return *devices[1];
        }

        static PATAChannel &primary() {
            return *ins_primary;
        }

        static PATAChannel &secondary() {
            return *ins_secondary;
        }
    };



}

#endif //OS_CPP_PATACHANNEL_HPP
