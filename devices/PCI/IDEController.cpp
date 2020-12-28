////
//// Created by Harold on 2020/5/15.
////
//
//#include <devices/PCI/IDEController.hpp>
//#include <devices/PCI/PCIManager.hpp>
//
//namespace kernel {
//    IDEController *IDEController::ins{};
//
//    void IDEController::init() {
//        auto address = pcim.find_device(PCIFindTarget(1, 1));
//        if (!address.target_found()) {
//            PANIC("IDE controller not found!");
//        }
//        ins = new IDEController(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
//    }
//
//    uint8_t ide_print_error(uint32_t drive, uint8_t err) {
////            if (err == 0)
////                return err;
////
////            printk("IDE:");
////            if (err == 1) {printk("- Device Fault\n     "); err = 19;}
////            else if (err == 2) {
////                uint8_t st = ide_read_reg(ide_devices[drive].channel, ATA_REG_ERROR);
////                if (st & ATA_ER_AMNF)   {printk("- No Address Mark Found\n     ");   err = 7;}
////                if (st & ATA_ER_TK0NF)   {printk("- No Media or Media Error\n     ");   err = 3;}
////                if (st & ATA_ER_ABRT)   {printk("- Command Aborted\n     ");      err = 20;}
////                if (st & ATA_ER_MCR)   {printk("- No Media or Media Error\n     ");   err = 3;}
////                if (st & ATA_ER_IDNF)   {printk("- ID mark not Found\n     ");      err = 21;}
////                if (st & ATA_ER_MC)   {printk("- No Media or Media Error\n     ");   err = 3;}
////                if (st & ATA_ER_UNC)   {printk("- Uncorrectable Data Error\n     ");   err = 22;}
////                if (st & ATA_ER_BBK)   {printk("- Bad Sectors\n     ");       err = 13;}
////            } else  if (err == 3)           {printk("- Reads Nothing\n     "); err = 23;}
////            else  if (err == 4)  {printk("- Write Protected\n     "); err = 8;}
////            printk("- [%s %s] %s\n",
////                   (const char *[]){"Primary", "Secondary"}[ide_devices[drive].channel], // Use the channel as an index into the array
////                    (const char *[]){"Master", "Slave"}[ide_devices[drive].drive], // Same as above, using the drive
////            ide_devices[drive].model);
////
////            return err;
//    }
//
//    uint8_t IDEController::ide_read_reg(uint8_t channel, uint8_t reg) {
//        uint8_t result = 0;
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_intr);
//        if (reg < 0x08)
//            result = inb(channels[channel].io_base + reg - 0x00);
//        else if (reg < 0x0C)
//            result = inb(channels[channel].io_base + reg - 0x06);
//        else if (reg < 0x0E)
//            result = inb(channels[channel].control_base + reg - 0x0A);
//        else if (reg < 0x16)
//            result = inb(channels[channel].bus_master_ide + reg - 0x0E);
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, channels[channel].no_intr);
//        return result;
//    }
//
//    void IDEController::ide_read_data(uint8_t channel, uint8_t reg, uint8_t *buffer,
//                                      uint32_t quads) {
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_intr);
//        if (reg < 0x08)
//            insl(channels[channel].io_base + reg - 0x00, buffer, quads);
//        else if (reg < 0x0C)
//            insl(channels[channel].io_base + reg - 0x06, buffer, quads);
//        else if (reg < 0x0E)
//            insl(channels[channel].control_base + reg - 0x0A, buffer, quads);
//        else if (reg < 0x16)
//            insl(channels[channel].bus_master_ide + reg - 0x0E, buffer, quads);
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, channels[channel].no_intr);
//    }
//
//    void IDEController::ide_write_reg(uint8_t channel, uint8_t reg, uint8_t data) {
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_intr);
//        if (reg < 0x08)
//            outb(channels[channel].io_base + reg - 0x00, data);
//        else if (reg < 0x0C)
//            outb(channels[channel].io_base + reg - 0x06, data);
//        else if (reg < 0x0E)
//            outb(channels[channel].control_base + reg - 0x0A, data);
//        else if (reg < 0x16)
//            outb(channels[channel].bus_master_ide + reg - 0x0E, data);
//        if (reg > 0x07 && reg < 0x0C)
//            ide_write_reg(channel, ATA_REG_CONTROL, channels[channel].no_intr);
//    }
//
//    uint8_t IDEController::ide_polling(uint8_t channel, bool advanced_check) {
//
//        // (I) Delay 400 nanosecond for BSY to be set:
//        // -------------------------------------------------
//        for (int i = 0; i < 4; i++)
//            ide_read_reg(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.
//
//        // (II) Wait for BSY to be cleared:
//        // -------------------------------------------------
//        while (ide_read_reg(channel, ATA_REG_STATUS) & ATA_SR_BSY); // Wait for BSY to be zero.
//
//        if (advanced_check) {
//            uint8_t state = ide_read_reg(channel, ATA_REG_STATUS); // Read Status Register.
//
//            // (III) Check For Errors:
//            // -------------------------------------------------
//            if (state & ATA_SR_ERR)
//                return 2; // Error.
//
//            // (IV) Check If Device fault:
//            // -------------------------------------------------
//            if (state & ATA_SR_DF)
//                return 1; // Device Fault.
//
//            // (V) Check DRQ:
//            // -------------------------------------------------
//            // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
//            if ((state & ATA_SR_DRQ) == 0)
//                return 3; // DRQ should be set
//
//        }
//
//        return 0; // No Error.
//
//    }
//
//    IDEController::IDEController(uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3, uint32_t BAR4) {
//        int j, k, count = 0;
//
//        // 1- Detect I/O Ports which interface IDE Controller:
//        channels[ATA_PRIMARY].io_base = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
//        channels[ATA_PRIMARY].control_base = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
//        channels[ATA_SECONDARY].io_base = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
//        channels[ATA_SECONDARY].control_base = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
//        channels[ATA_PRIMARY].bus_master_ide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
//        channels[ATA_SECONDARY].bus_master_ide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE
//
//        // 2- Disable IRQs:
//        ide_write_reg(ATA_PRIMARY, ATA_REG_CONTROL, 2);
//        ide_write_reg(ATA_SECONDARY, ATA_REG_CONTROL, 2);
//        // 3- Detect ATA-ATAPI Devices:
//        for (int i = 0; i < 2; i++)
//            for (j = 0; j < 2; j++) {
//
//                uint8_t err = 0, type = IDE_ATA, status = 0;
//                ide_devices[count].exists = 0; // Assuming that no drive here.
//
//                // (I) Select Drive:
//                ide_write_reg(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
////                sleep(1); // Wait 1ms for drive select to work.  TODO
//
//                // (II) Send ATA Identify Command:
//                ide_write_reg(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
////                sleep(1); // This function should be implemented in your OS. which waits for 1 ms. TODO
//                // it is based on System Timer Device Driver.
//
//                // (III) Polling:
//                if (ide_read_reg(i, ATA_REG_STATUS) == 0) continue; // If Status = 0, No Device.
//
//                while (true) {
//                    status = ide_read_reg(i, ATA_REG_STATUS);
//                    if ((status & ATA_SR_ERR)) {
//                        err = 1;
//                        break;
//                    } // If Err, Device is not ATA.
//                    if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
//                }
//
//                // (IV) Probe for ATAPI Devices:
//
//                if (err != 0) {
//                    uint8_t cl = ide_read_reg(i, ATA_REG_LBA1);
//                    uint8_t ch = ide_read_reg(i, ATA_REG_LBA2);
//
//                    if (cl == 0x14 && ch == 0xEB)
//                        type = IDE_ATAPI;
//                    else if (cl == 0x69 && ch == 0x96)
//                        type = IDE_ATAPI;
//                    else
//                        continue; // Unknown Type (may not be a device).
//
//                    ide_write_reg(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
////                    sleep(1); TODO
//                }
//
//                // (V) Read Identification Space of the Device:
//                ide_read_data(i, ATA_REG_DATA, ide_buf, 128);
//
//                // (VI) Read Device Parameters:
//                ide_devices[count].exists = 1;
//                ide_devices[count].type = type;
//                ide_devices[count].channel = i;
//                ide_devices[count].slave = j;
//                ide_devices[count].signature = *((uint16_t *) (ide_buf + ATA_IDENT_DEVICETYPE));
//                ide_devices[count].capabilities = *((uint16_t *) (ide_buf + ATA_IDENT_CAPABILITIES));
//                ide_devices[count].command_sets = *((uint32_t *) (ide_buf + ATA_IDENT_COMMANDSETS));
//
//                // (VII) Get Size:
//                if (ide_devices[count].command_sets & (1 << 26))
//                    // Device uses 48-Bit Addressing:
//                    ide_devices[count].size = *((uint32_t *) (ide_buf + ATA_IDENT_MAX_LBA_EXT));
//                else
//                    // Device uses CHS or 28-bit Addressing:
//                    ide_devices[count].size = *((uint32_t *) (ide_buf + ATA_IDENT_MAX_LBA));
//
//                // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
//                for (k = 0; k < 40; k += 2) {
//                    ide_devices[count].model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
//                    ide_devices[count].model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
//                }
//                ide_devices[count].model[40] = 0; // Terminate String.
//
//                count++;
//            }
//
//        // 4- Print Summary:
//        for (auto &ide_device : ide_devices)
//            if (ide_device.exists == 1) {
//                kprint(" Found %s Drive %dGB - %s\n",
//                       (const char *[]) {"ATA", "ATAPI"}[ide_device.type],         /* Type */
//                        ide_device.size / 1024 / 1024 / 2,               /* Size */
//                        ide_device.model);
//            }
//    }
//
//    uint8_t IDEController::ide_ata_access(uint8_t write, uint8_t drive, uint32_t lba,
//                                          uint8_t sector_num, uint8_t *buffer) {
//        uint8_t lba_mode /* 0: CHS, 1:LBA28, 2: LBA48 */, dma /* 0: No DMA, 1: DMA */, cmd;
//        uint8_t lba_io[6];
//        uint32_t channel = ide_devices[drive].channel; // Read the Channel.
//        uint32_t slavebit = ide_devices[drive].slave; // Read the Drive [Master/Slave]
//        uint32_t bus = channels[channel].io_base; // Bus Base, like 0x1F0 which is also data port.
//        uint32_t words = 256; // Almost every ATA drive has a sector-size of 512-byte.
//        uint16_t cyl, i;
//        uint8_t head, sect, err;
//        // disable irq
//        ide_irq_invoked = false;
//        ide_write_reg(channel, ATA_REG_CONTROL, channels[channel].no_intr = 0x02);
//        // (I) Select one from LBA28, LBA48 or CHS;
//        if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are
//            // giving a wrong LBA.
//            // LBA48:
//            lba_mode = 2;
//            lba_io[0] = (lba & 0x000000FF) >> 0;
//            lba_io[1] = (lba & 0x0000FF00) >> 8;
//            lba_io[2] = (lba & 0x00FF0000) >> 16;
//            lba_io[3] = (lba & 0xFF000000) >> 24;
//            lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
//            lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
//            head = 0; // Lower 4-bits of HDDEVSEL are not used here.
//        } else if (ide_devices[drive].capabilities & 0x200) { // Drive supports LBA?
//            // LBA28:
//            lba_mode = 1;
//            lba_io[0] = (lba & 0x00000FF) >> 0;
//            lba_io[1] = (lba & 0x000FF00) >> 8;
//            lba_io[2] = (lba & 0x0FF0000) >> 16;
//            lba_io[3] = 0; // These Registers are not used here.
//            lba_io[4] = 0; // These Registers are not used here.
//            lba_io[5] = 0; // These Registers are not used here.
//            head = (lba & 0xF000000) >> 24;
//        } else {
//            // CHS:
//            lba_mode = 0;
//            sect = (lba % 63) + 1;
//            cyl = (lba + 1 - sect) / (16 * 63);
//            lba_io[0] = sect;
//            lba_io[1] = (cyl >> 0) & 0xFF;
//            lba_io[2] = (cyl >> 8) & 0xFF;
//            lba_io[3] = 0;
//            lba_io[4] = 0;
//            lba_io[5] = 0;
//            head = (lba + 1 - sect) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
//        }
//        // (II) See if drive supports DMA or not;
//        dma = 0; // We don't support DMA TODO
//
//        // (III) Wait if the drive is busy;
//        while (ide_read_reg(channel, ATA_REG_STATUS) & ATA_SR_BSY); // Wait if busy.
//        /*
//          The HDDDEVSEL register now looks like this:
//          Bits 0 : 3: Head Number for CHS.
//          Bit 4: Slave Bit. (0: Selecting Master Drive, 1: Selecting Slave Drive).
//          Bit 5: Obsolete and isn't used, but should be set.
//          Bit 6: LBA (0: CHS, 1: LBA).
//          Bit 7: Obsolete and isn't used, but should be set.
//         */
//        // (IV) Select Drive from the controller;
//        if (lba_mode == 0)
//            ide_write_reg(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head); // Drive & CHS.
//        else
//            ide_write_reg(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head); // Drive & LBA
//        // (V) Write Parameters;
//        if (lba_mode == 2) {
//            ide_write_reg(channel, ATA_REG_SECCOUNT1, 0);
//            ide_write_reg(channel, ATA_REG_LBA3, lba_io[3]);
//            ide_write_reg(channel, ATA_REG_LBA4, lba_io[4]);
//            ide_write_reg(channel, ATA_REG_LBA5, lba_io[5]);
//        }
//        ide_write_reg(channel, ATA_REG_SECCOUNT0, sector_num);
//        ide_write_reg(channel, ATA_REG_LBA0, lba_io[0]);
//        ide_write_reg(channel, ATA_REG_LBA1, lba_io[1]);
//        ide_write_reg(channel, ATA_REG_LBA2, lba_io[2]);
//        // (VI) Select the command and send it;
//        // Routine that is followed:
//        // If ( DMA & LBA48)   DO_DMA_EXT;
//        // If ( DMA & LBA28)   DO_DMA_LBA;
//        // If ( DMA & LBA28)   DO_DMA_CHS;
//        // If (!DMA & LBA48)   DO_PIO_EXT;
//        // If (!DMA & LBA28)   DO_PIO_LBA;
//        // If (!DMA & !LBA#)   DO_PIO_CHS;
//        if (lba_mode == 0 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO;
//        if (lba_mode == 1 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO;
//        if (lba_mode == 2 && dma == 0 && write == 0) cmd = ATA_CMD_READ_PIO_EXT;
//        if (lba_mode == 0 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA;
//        if (lba_mode == 1 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA;
//        if (lba_mode == 2 && dma == 1 && write == 0) cmd = ATA_CMD_READ_DMA_EXT;
//        if (lba_mode == 0 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO;
//        if (lba_mode == 1 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO;
//        if (lba_mode == 2 && dma == 0 && write == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
//        if (lba_mode == 0 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA;
//        if (lba_mode == 1 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA;
//        if (lba_mode == 2 && dma == 1 && write == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
//        ide_write_reg(channel, ATA_REG_COMMAND, cmd);               // Send the Command.
//        if (dma)
//            if (write == 0);
//                // DMA Read. TODO
//            else;
//            // DMA Write. TODO
//        else if (write == 0)
//            // PIO Read.
//            for (i = 0; i < sector_num; i++) {
//                err = ide_polling(channel, 1);
//                if (err)
//                    return err; // Polling, set error and exit if there is.
//                insw(bus, buffer, words);
//                buffer += words * 2;
////                asm volatile ("rep insw"
////                              ""
////                              "");
////                asm("rep insw" : : "c"(words), "d"(bus), "D"(edi)); // Receive Data.
////                edi += (words * 2);
//            }
//        else {
//            // PIO Write.
//            for (i = 0; i < sector_num; i++) {
//                ide_polling(channel, 0); // Polling.
//                outsw(bus, buffer, words);
//                buffer += words * 2;
////                asm("pushw %ds");
////                asm("mov %%ax, %%ds"::"a"(selector));
////                asm("rep outsw"::"c"(words), "d"(bus), "S"(edi)); // Send Data
////                asm("popw %ds");
////                edi += (words * 2);
//            }
//            uint8_t modes[] = {ATA_CMD_CACHE_FLUSH,
//                               ATA_CMD_CACHE_FLUSH,
//                               ATA_CMD_CACHE_FLUSH_EXT};
//            ide_write_reg(channel, ATA_REG_COMMAND, modes[lba_mode]);
//            ide_polling(channel, 0); // Polling.
//        }
//
//        return 0; // Easy, isn't it?
//    }
//
//    void IDEController::ide_read_sectors(uint8_t drive, uint8_t sector_count, uint32_t lba, uint8_t *buffer) {
//
//    }
//
//
//// package[0] is an entry of an array. It contains the Error Code.
//
//}
//
