////
//// Created by Harold on 2020/5/15.
////
//
//#ifndef OS_CPP_IDECONTROLLER_HPP
//#define OS_CPP_IDECONTROLLER_HPP
//
//#include <devices/PCI/PCIDevice.hpp>
//#include <arch/x86/io.hpp>
///*From wiki.osdev.org*/
//// Status
//#define ATA_SR_BSY     0x80    // Busy
//#define ATA_SR_DRDY    0x40    // Drive ready
//#define ATA_SR_DF      0x20    // Drive write fault
//#define ATA_SR_DSC     0x10    // Drive seek complete
//#define ATA_SR_DRQ     0x08    // Data request ready
//#define ATA_SR_CORR    0x04    // Corrected data
//#define ATA_SR_IDX     0x02    // Index
//#define ATA_SR_ERR     0x01    // Error
//// Errors
//#define ATA_ER_BBK      0x80    // Bad block
//#define ATA_ER_UNC      0x40    // Uncorrectable data
//#define ATA_ER_MC       0x20    // Media changed
//#define ATA_ER_IDNF     0x10    // ID mark not found
//#define ATA_ER_MCR      0x08    // Media change request
//#define ATA_ER_ABRT     0x04    // Command aborted
//#define ATA_ER_TK0NF    0x02    // Track 0 not found
//#define ATA_ER_AMNF     0x01    // No address mark
//
////Commands
//#define ATA_CMD_READ_PIO          0x20
//#define ATA_CMD_READ_PIO_EXT      0x24
//#define ATA_CMD_READ_DMA          0xC8
//#define ATA_CMD_READ_DMA_EXT      0x25
//#define ATA_CMD_WRITE_PIO         0x30
//#define ATA_CMD_WRITE_PIO_EXT     0x34
//#define ATA_CMD_WRITE_DMA         0xCA
//#define ATA_CMD_WRITE_DMA_EXT     0x35
//#define ATA_CMD_CACHE_FLUSH       0xE7
//#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
//#define ATA_CMD_PACKET            0xA0
//#define ATA_CMD_IDENTIFY_PACKET   0xA1
//#define ATA_CMD_IDENTIFY          0xEC
//#define ATAPI_CMD_READ       0xA8
//#define ATAPI_CMD_EJECT      0x1B
//#define ATA_IDENT_DEVICETYPE   0
//#define ATA_IDENT_CYLINDERS    2
//#define ATA_IDENT_HEADS        6
//#define ATA_IDENT_SECTORS      12
//#define ATA_IDENT_SERIAL       20
//#define ATA_IDENT_MODEL        54
//#define ATA_IDENT_CAPABILITIES 98
//#define ATA_IDENT_FIELDVALID   106
//#define ATA_IDENT_MAX_LBA      120
//#define ATA_IDENT_COMMANDSETS  164
//#define ATA_IDENT_MAX_LBA_EXT  200
//#define IDE_ATA        0x00
//#define IDE_ATAPI      0x01
//#define ATA_MASTER     0x00
//#define ATA_SLAVE      0x01
//#define ATA_REG_DATA       0x00
//#define ATA_REG_ERROR      0x01
//#define ATA_REG_FEATURES   0x01
//#define ATA_REG_SECCOUNT0  0x02
//#define ATA_REG_LBA0       0x03
//#define ATA_REG_LBA1       0x04
//#define ATA_REG_LBA2       0x05
//#define ATA_REG_HDDEVSEL   0x06
//#define ATA_REG_COMMAND    0x07
//#define ATA_REG_STATUS     0x07
//#define ATA_REG_SECCOUNT1  0x08
//#define ATA_REG_LBA3       0x09
//#define ATA_REG_LBA4       0x0A
//#define ATA_REG_LBA5       0x0B
//#define ATA_REG_CONTROL    0x0C
//#define ATA_REG_ALTSTATUS  0x0C
//#define ATA_REG_DEVADDRESS 0x0D
//// Channels:
//#define      ATA_PRIMARY      0x00
//#define      ATA_SECONDARY    0x01
//
//// Directions:
//#define      ATA_READ      0x00
//#define      ATA_WRITE     0x01
//
//namespace kernel {
//    struct IDEDevice {
//        uint8_t exists;    // 0 (Empty) or 1 (This Drive really exists).
//        uint8_t channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
//        uint8_t slave;       // 0 (Master Drive) or 1 (Slave Drive).
//        uint16_t type;        // 0: ATA, 1:ATAPI.
//        uint16_t signature;   // Drive Signature
//        uint16_t capabilities;// Features.
//        uint32_t command_sets; // Command Sets Supported.
//        uint32_t size;        // Size in Sectors.
//        uint8_t model[41];   // Model in string.
//    } ide_devices[4];
//
//    struct IDEChannel {
//        uint16_t io_base;
//        uint16_t control_base;
//        uint16_t bus_master_ide;
//        uint8_t no_intr;
//    };
//
//    class IDEController : PCIDevice {
//        IDEController(uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3, uint32_t BAR4);
//        static IDEController *ins;
//        IDEChannel channels[2]{};
//        uint8_t ide_buf[2048]{0};
//        bool ide_irq_invoked{false};
//        uint8_t atapi_packet[12] = {0xA8};
//
//
//        uint8_t ide_read_reg(uint8_t channel, uint8_t reg);
//
//        void ide_write_reg(uint8_t channel, uint8_t reg, uint8_t data);
//
//        void ide_read_data(uint8_t channel, uint8_t reg, uint8_t *buffer,
//                           uint32_t quads);
//
//        uint8_t ide_ata_access(uint8_t write, uint8_t drive, uint32_t lba,
//                               uint8_t sector_num, uint8_t *buffer);
//
//        uint8_t ide_polling(uint8_t channel, bool advanced_check);
//
//        uint8_t ide_print_error(uint8_t drive, uint8_t err);
//
//        void ide_read_sectors(uint8_t drive, uint8_t sector_count, uint32_t lba,
//                              uint8_t *buffer);
//
//    public:
//        static void init();
//
//    };
//
//
//}
//
//#endif //OS_CPP_IDECONTROLLER_HPP
