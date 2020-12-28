//
// Created by Harold on 2020/5/16.
//

#include <devices/PATAChannel.hpp>
#include <arch/x86/io.hpp>
#include <devices/PCI/PCIManager.hpp>
#include <devices/PATADevice.hpp>
/* From https://wiki.osdev.org/PCI_IDE_Controller */

namespace kernel {
    PATAChannel *PATAChannel::ins_primary{};
    PATAChannel *PATAChannel::ins_secondary{};

    uint8_t PATAChannel::read_reg(uint8_t reg) {
        uint8_t result = 0;
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, 0x80 | no_intr);
        if (reg < 0x08)
            result = inb(io_base + reg - 0x00);
        else if (reg < 0x0C)
            result = inb(io_base + reg - 0x06);
        else if (reg < 0x0E)
            result = inb(control_base + reg - 0x0A);
        else if (reg < 0x16)
            result = inb(bus_master_ide + reg - 0x0E);
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, no_intr);
        return result;
    }

    void PATAChannel::write_reg(uint8_t reg, uint8_t data) {
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, 0x80 | no_intr);
        if (reg < 0x08)
            outb(io_base + reg - 0x00, data);
        else if (reg < 0x0C)
            outb(io_base + reg - 0x06, data);
        else if (reg < 0x0E)
            outb(control_base + reg - 0x0A, data);
        else if (reg < 0x16)
            outb(bus_master_ide + reg - 0x0E, data);
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, no_intr);
    }

    void PATAChannel::read_reg_data(uint8_t reg, uint8_t *buffer,
                                    uint32_t dw_count) {
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, 0x80 | no_intr);
        if (reg < 0x08)
            insl(io_base + reg - 0x00, buffer, dw_count);
        else if (reg < 0x0C)
            insl(io_base + reg - 0x06, buffer, dw_count);
        else if (reg < 0x0E)
            insl(control_base + reg - 0x0A, buffer, dw_count);
        else if (reg < 0x16)
            insl(bus_master_ide + reg - 0x0E, buffer, dw_count);
        if (reg > 0x07 && reg < 0x0C)
            write_reg(ATA_REG_CONTROL, no_intr);
    }


    uint8_t PATAChannel::polling(bool advanced_check) {
        // (I) Delay 400 nanosecond for BSY to be set:
        // -------------------------------------------------
        for (int i = 0; i < 4; i++)
            read_reg(ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.
        // (II) Wait for BSY to be cleared:
        // -------------------------------------------------
        while (read_reg(ATA_REG_STATUS) & ATA_SR_BSY); // Wait for BSY to be zero.
        if (advanced_check) {
            uint8_t state = read_reg(ATA_REG_STATUS); // Read Status Register.
            // (III) Check For Errors:
            // -------------------------------------------------
            if (state & ATA_SR_ERR)
                return 2; // Error.
            // (IV) Check If Device fault:
            // ---------------- ---------------------------------
            if (state & ATA_SR_DF)
                return 1; // Device Fault.
            // (V) Check DRQ:
            // -------------------------------------------------
            // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
            if ((state & ATA_SR_DRQ) == 0)
                return 3; // DRQ should be set
        }
        return 0; // No Error.
    }

    ssize_t PATAChannel::ata_access(bool write, bool dma, PATADeviceDriveType type, uint32_t lba, uint8_t sector_num,
                                uint8_t *buffer) {
        uint8_t lba_mode{}; /* 0: CHS, 1:LBA28, 2: LBA48 */
        uint8_t cmd{};
        uint8_t lba_io[6];
        auto *device = devices[type];
        uint32_t slave_bit = static_cast<uint8_t>(device->drive_type_); // Read the Drive [Master/Slave]
        uint32_t bus = io_base; // Bus Base, like 0x1F0 which is also data port.
        uint32_t words = 256; // Almost every ATA drive has a sector-size of 512-byte.
        uint16_t cyl{}, i{};
        uint8_t head{}, sector{}, err{};

        // disable irq
        irq_invoked = false;
        write_reg(ATA_REG_CONTROL, no_intr = 0x02);
        // (I) Select one from LBA28, LBA48 or CHS;
        if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are
            // giving a wrong LBA.
            // LBA48:
            lba_mode = 2;
            lba_io[0] = (lba & 0x000000FF) >> 0;
            lba_io[1] = (lba & 0x0000FF00) >> 8;
            lba_io[2] = (lba & 0x00FF0000) >> 16;
            lba_io[3] = (lba & 0xFF000000) >> 24;
            lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
            lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
            head = 0; // Lower 4-bits of HDDEVSEL are not used here.
        } else if (device->capabilities & 0x200) { // Drive supports LBA?
            // LBA28:
            lba_mode = 1;
            lba_io[0] = (lba & 0x00000FF) >> 0;
            lba_io[1] = (lba & 0x000FF00) >> 8;
            lba_io[2] = (lba & 0x0FF0000) >> 16;
            lba_io[3] = lba_io[4] = lba_io[5] = 0;
            head = (lba & 0xF000000) >> 24;
        } else {
            // CHS:
            lba_mode = 0;
            sector = (lba % 63) + 1;
            cyl = (lba + 1 - sector) / (16 * 63);
            lba_io[0] = sector;
            lba_io[1] = (cyl >> 0) & 0xFF;
            lba_io[2] = (cyl >> 8) & 0xFF;
            lba_io[3] = lba_io[4] = lba_io[5] = 0;
            head = (lba + 1 - sector) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
        }
        // (II) See if drive supports DMA or not;

        // (III) Wait if the drive is busy;
        while (read_reg(ATA_REG_STATUS) & ATA_SR_BSY); // Wait if busy.
        /*
          The HDDDEVSEL register now looks like this:
          Bits 0 : 3: Head Number for CHS.
          Bit 4: Slave Bit. (0: Selecting Master Drive, 1: Selecting Slave Drive).
          Bit 5: Obsolete and isn't used, but should be set.
          Bit 6: LBA (0: CHS, 1: LBA).
          Bit 7: Obsolete and isn't used, but should be set.
         */
        // (IV) Select Drive from the controller;
        if (lba_mode == 0)
            write_reg(ATA_REG_HDDEVSEL, 0xA0 | (slave_bit << 4) | head); // Drive & CHS.
        else
            write_reg(ATA_REG_HDDEVSEL, 0xE0 | (slave_bit << 4) | head); // Drive & LBA
        // (V) Write Parameters;
        if (lba_mode == 2) {
            write_reg(ATA_REG_SECCOUNT1, 0);
            write_reg(ATA_REG_LBA3, lba_io[3]);
            write_reg(ATA_REG_LBA4, lba_io[4]);
            write_reg(ATA_REG_LBA5, lba_io[5]);
        }
        write_reg(ATA_REG_SECCOUNT0, sector_num);
        write_reg(ATA_REG_LBA0, lba_io[0]);
        write_reg(ATA_REG_LBA1, lba_io[1]);
        write_reg(ATA_REG_LBA2, lba_io[2]);
        // (VI) Select the command and send it;
        // Routine that is followed:
        // If ( DMA & LBA48)   DO_DMA_EXT;
        // If ( DMA & LBA28)   DO_DMA_LBA;
        // If ( DMA & LBA28)   DO_DMA_CHS;
        // If (!DMA & LBA48)   DO_PIO_EXT;
        // If (!DMA & LBA28)   DO_PIO_LBA;
        // If (!DMA & !LBA#)   DO_PIO_CHS;

        if (!dma) {
            if (lba_mode == 2)
                cmd = write ? ATA_CMD_WRITE_PIO_EXT : ATA_CMD_READ_PIO_EXT;
            else
                cmd = write ? ATA_CMD_WRITE_PIO : ATA_CMD_READ_PIO;
        } else {
            if (lba_mode == 2)
                cmd = write ? ATA_CMD_WRITE_DMA_EXT : ATA_CMD_READ_DMA_EXT;
            else
                cmd = write ? ATA_CMD_WRITE_DMA : ATA_CMD_READ_DMA;
        }

        write_reg(ATA_REG_COMMAND, cmd);               // Send the Command.
        if (dma)
            if (!write) {
                // DMA Read. TODO
            } else;
            // DMA Write. TODO
        else if (!write)
            // PIO Read.
            for (i = 0; i < sector_num; i++) {
                err = polling(true);
                if (err)
                    return err; // Polling, set error and exit if there is.
                insw(bus, buffer, words);
                buffer += words * 2;
//                asm volatile ("rep insw"
//                              ""
//                              "");
//                asm("rep insw" : : "c"(words), "d"(bus), "D"(edi)); // Receive Data.
//                edi += (words * 2);
            }
        else {
            // PIO Write.
            for (i = 0; i < sector_num; i++) {
                polling(false); // Polling.
                outsw(bus, buffer, words);
                buffer += words * 2;
//                asm("pushw %ds");
//                asm("mov %%ax, %%ds"::"a"(selector));
//                asm("rep outsw"::"c"(words), "d"(bus), "S"(edi)); // Send Data
//                asm("popw %ds");
//                edi += (words * 2);
            }
            uint8_t mode = lba_mode == 2 ? ATA_CMD_CACHE_FLUSH_EXT : ATA_CMD_CACHE_FLUSH;
            write_reg(ATA_REG_COMMAND, mode);
            polling(false); // Polling.
        }

        return 0; // Easy, isn't it?
    }

    ssize_t PATAChannel::read_sectors(PATADeviceDriveType type, uint8_t sector_count, uint32_t lba, uint8_t *buffer) {
        // 1: Check if the drive presents:
        // ==================================
        auto &device = devices[type];
        if (!device->exists_) {
            return ATA_ERR_DEVICE_NOT_FOUND;
        }
        if (((lba + sector_count) > device->size) && (device->type == IDE_ATA)) {
            return ATA_ERR_INVALID_SEEK;
        } else {
            uint8_t err{};
            if (device->type == IDE_ATA)
                err = ata_access(false, false, type, lba, sector_count, buffer);
            else if (device->type == IDE_ATAPI) {
                // TODO
//                for (i = 0; i < numsects; i++)
//                    err = ide_atapi_read(drive, lba + i, 1, es, edi + (i*2048));
//                package[0] = ide_print_error(drive, err);
            }
            return err;
        }
    }

    PATAChannel::PATAChannel(PATAChannelType type, uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3,
                             uint32_t BAR4) : type(type) {
        // 1- Detect I/O Ports which interface IDE Controller:
        if (type == PATAChannelType::PRIMARY) {
            io_base = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
            control_base = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
            bus_master_ide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
        } else {
            io_base = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
            control_base = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
            bus_master_ide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE
        }
    }

    void PATAChannel::init() {
        auto address = pcim.find_device(PCIFindTarget(1, 1));
        if (!address.target_found()) {
            PANIC("IDE controller not found!");
        }
        ins_primary = new PATAChannel(PATAChannelType::PRIMARY, 0x1F0, 0x3F6, 0x170, 0x376, 0x000);;
        ins_secondary = new PATAChannel(PATAChannelType::SECONDARY, 0x1F0, 0x3F6, 0x170, 0x376, 0x000);
        // 2- Disable IRQs:
        ins_primary->write_reg(ATA_REG_CONTROL, 2);
        ins_secondary->write_reg(ATA_REG_CONTROL, 2);
        // 3- Detect ATA-ATAPI Devices:
        auto *ptr = ins_primary;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                uint8_t err = 0, type = IDE_ATA, status = 0;
//                ide_devices[count].exists = 0; // Assuming that no drive here.
                // (I) Select Drive:
                ptr->write_reg(ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
//                sleep(1); // Wait 1ms for drive select to work.  TODO

                // (II) Send ATA Identify Command:
                ptr->write_reg(ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
//                sleep(1); // This function should be implemented in your OS. which waits for 1 ms. TODO
                // it is based on System Timer Device Driver.
                // (III) Polling:
                if (ptr->read_reg(ATA_REG_STATUS) == 0) continue; // If Status = 0, No device->
                while (true) {
                    status = ptr->read_reg(ATA_REG_STATUS);
                    if ((status & ATA_SR_ERR)) {
                        err = 1;
                        break;
                    } // If Err, Device is not ATA.
                    if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ))
                        break; // Everything is right.
                }

                // (IV) Probe for ATAPI Devices:
                if (err != 0) {
                    uint8_t cl = ptr->read_reg(ATA_REG_LBA1);
                    uint8_t ch = ptr->read_reg(ATA_REG_LBA2);

                    if ((cl == 0x14 && ch == 0xEB) || (cl == 0x69 && ch == 0x96))
                        type = IDE_ATAPI;
                    else {
                        type = IDE_UNKNOWN;
                        continue; // Unknown Type (may not be a device).
                    }
                    ptr->write_reg(ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
//                    sleep(1); TODO
                }

                // (V) Read Identification Space of the Device:
                ptr->read_reg_data(ATA_REG_DATA, ptr->id_buffer, 128);

                // (VI) Read Device Parameters:
                ptr->devices[j] = new PATADevice(*ptr);
                auto *device = ptr->devices[j];
                device->exists_ = 1;
                device->type = type;
                device->channel_num = i;
                device->drive_type_ = static_cast<PATADeviceDriveType>(j);
                device->signature = *((uint16_t *) (ptr->id_buffer + ATA_IDENT_DEVICETYPE));
                device->capabilities = *((uint16_t *) (ptr->id_buffer + ATA_IDENT_CAPABILITIES));
                device->command_sets = *((uint32_t *) (ptr->id_buffer + ATA_IDENT_COMMANDSETS));
                // (VII) Get Size:
                if (device->command_sets & (1 << 26))
                    // Device uses 48-Bit Addressing:
                    device->size = *((uint32_t *) (ptr->id_buffer + ATA_IDENT_MAX_LBA_EXT));
                else
                    // Device uses CHS or 28-bit Addressing:
                    device->size = *((uint32_t *) (ptr->id_buffer + ATA_IDENT_MAX_LBA));

                // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
                for (int k = 0; k < 40; k += 2) {
                    device->model[k] = ptr->id_buffer[ATA_IDENT_MODEL + k + 1];
                    device->model[k + 1] = ptr->id_buffer[ATA_IDENT_MODEL + k];
                }
                device->model[40] = 0; // Terminate String.
            }
            ptr = ins_secondary;
        }

    }
}

