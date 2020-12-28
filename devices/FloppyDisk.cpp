///*
// * Copyright (c) 2019-2020, Jesse Buhagiar <jooster669@gmail.com>
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without
// * modification, are permitted provided that the following conditions are met:
// *
// * 1. Redistributions of source code must retain the above copyright notice, this
// *    list of conditions and the following disclaimer.
// *
// * 2. Redistributions in binary form must reproduce the above copyright notice,
// *    this list of conditions and the following disclaimer in the documentation
// *    and/or other materials provided with the distribution.
// *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// */
//
//#include <devices/FloppyDisk.hpp>
//#include <kernel/tasking/scheduler.hpp>
//#include <sys/asm.hpp>
//#include <kernel/memory/MemoryManager.hpp>
//#include <libcpp/cstring.hpp>
//
//namespace kernel {
//
////#define FLOPPY_DEBUG
//
//// THESE ARE OFFSETS!
//#define FLOPPY_STATUS_A 0x00 // ro
//#define FLOPPY_STATUS_B 0x01 // ro
//#define FLOPPY_DOR 0x02      // rw
//#define FLOPPY_TDR 0x03      // rw
//#define FLOPPY_MSR 0x04      // ro
//#define FLOPPY_DSR 0x04      // wo
//#define FLOPPY_FIFO 0x05
//#define FLOPPY_RSVD 0x06
//#define FLOPPY_DIR 0x07 // ro
//#define FLOPPY_CCR 0x07 // wo
//
//#define FLOPPY_STATUS_DIR 0x01
//#define FLOPPY_STATUS_WP 0x02
//#define FLOPPY_STATUS_INDX 0x04
//#define FLOPPY_STATUS_HDSEL 0x08
//#define FLOPPY_STATUS_TRK0 0x10
//#define FLOPPY_STATUS_STEP 0x20
//#define FLOPPY_STATUS_DRV2 0x40
//#define FLOPPY_STATUS_INTW 0x80 // A.K.A INT_PENDING
//
//#define FLOPPY_DOR_DRVSEL0 0x01
//#define FLOPPY_DOR_DRVSEL1 0x02
//#define FLOPPY_DOR_RESET 0x04
//#define FLOPPY_DOR_DMAGATE 0x08
//#define FLOPPY_DOR_MOTEN0 0x10
//#define FLOPPY_DOR_MOTEN1 0x20
//#define FLOPPY_DOR_MOTEN2 0x40
//#define FLOPPY_DOR_MOTEN3 0x80
//// Preset values to activate drive select and motor enable for each drive
//#define FLOPPY_DOR_DRV0 0x1C
//#define FLOPPY_DOR_DRV1 0x2D
//#define FLOPPY_DOR_DRV2 0x4E
//#define FLOPPY_DOR_DRV3 0x8F
//
//#define FLOPPY_MSR_FDD0BSY 0x01
//#define FLOPPY_MSR_FDD1BSY 0x02
//#define FLOPPY_MSR_FDD2BSY 0x04
//#define FLOPPY_MSR_FDD3BSY 0x08
//#define FLOPPY_MSR_FDCBSY 0x10
//#define FLOPPY_MSR_MODE 0x20 // 0 in DMA mode, 1 in PIO mode
//#define FLOPPY_MSR_DIO 0x40  // 0 FDC is expecting data from the CPU, 1 if FDC has data for CPU
//#define FLOPPY_MSR_RQM 0x80  // 0 Data register not ready, 1 data register ready
//
//#define FLOPPY_CCR_DRTESEL0 0x01
//#define FLOPPY_CCR_DRTESEL1 0x02
//
//#define FLOPPY_MT 0x80  // Multi-track selector. The controller treats 2 tracks (on side 0 and side 1) as a single track instead
//#define FLOPPY_MFM 0x40 // 1 Means this disk is double density (double sided??)
//#define FLOPPY_SK 0x20  // Skip flag. Skips sectors containing deleted data automatically for us :)
//
//#define SR0_OKAY (0x00) << 6
//#define SR0_ABORMAL_TERMINATION (0x01) << 6
//#define SR0_INVALID_CMD (0x02) << 6
//#define SR0_ABNORMAL_TERM_POLL (0x03) << 6
//
//#define FLOPPY_DMA_CHANNEL 2 // All FDCs are DMA channel 2
//#define IRQ_FLOPPY_DRIVE 6+32
////
////    NonnullRefPtr<FloppyDisk> FloppyDisk::create(DriveType type)
////    {
////        return adopt(*new FloppyDisk(type));
////    }
//
////    const char* FloppyDisk::class_name() const
////    {
////        if (m_controller_version == 0x90)
////            return "Intel 82078 Floppy Disk Controller";
////        else if (m_controller_version == 0x80)
////            return "NEC uPD765";
////
////        return "Generic Floppy Disk Controller";
////    }
//
//    FloppyDisk::FloppyDisk(FloppyDisk::DriveType type)
////            : BlockDevice(89, (type == FloppyDisk::DriveType::Master) ? 0 : 1, BYTES_PER_SECTOR)
//            : x86::IRQHandler(IRQ_FLOPPY_DRIVE), BlockDevice(BYTES_PER_SECTOR),
//              m_io_base_addr((type == FloppyDisk::DriveType::Master) ? 0x3F0 : 0x370) {
//        initialize();
//    }
//
//
//    int FloppyDisk::block_read(uint8_t *data, size_t index, size_t count) {
//        return read_sectors_with_dma(index, count, data);
//    }
//
//    int FloppyDisk::write_blocks(uint8_t *data, size_t index, size_t count) {
//        return write_sectors_with_dma(index, count, data);;
//    }
//
//    bool FloppyDisk::read_sectors_with_dma(uint16_t lba, uint16_t count, uint8_t *outbuf) {
//        m_lock.lock(); // Acquire lock
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: read_sectors_with_dma lba = " << lba << " count = " << count;
////#endif
//
//        motor_enable(slave()); // Should I bother casting this?!
//        write_ccr(0);
//        recalibrate();
//
//        if (!seek(lba)) {
////            klog() << "fdc: failed to seek to lba = " << lba << "!";
//            m_lock.unlock();
//            return false;
//        }
//
//        // We have to wait for about 300ms for the drive to spin up, because of
//        // the inertia of the motor and diskette. This is only
//        // important on real hardware
//        // TODO: Fix this if you want to get it running on real hardware. This code doesn't allow
//        // time for the disk to spin up.
//
//        //u32 start = PIT::seconds_since_boot();
//        //while(start < PIT::seconds_since_boot() + 1)
//        //    ;
//        x86::disable_irq();
//
//        outb(0xA, FLOPPY_DMA_CHANNEL | 0x4); // Channel 2 SEL, MASK_ON = 1
//        outb(0x0B, 0x56);                    // Begin DMA, Single Transfer, Increment, Auto, FDC -> RAM, Channel 2
//        outb(0xA, 0x2);                      // Unmask channel 2. The transfer will now begin
//
//        // Translate the LBA address into something the FDC understands.
//        uint16_t cylinder = lba2cylinder(lba);
//        uint16_t head = lba2head(lba);
//        uint16_t sector = lba2sector(lba);
//
//#ifdef FLOPPY_DEBUG
//        klog() << "fdc: addr = 0x" << String::write("%x", lba * BYTES_PER_SECTOR) << " c = " << cylinder << " h = " << head << " s = " << sector;
//#endif
//
//        // Intel recommends 3 attempts for a read/write
//        for (int i = 0; i < 3; i++) {
//            // Now actually send the command to the drive. This is a big one!
//            send_byte(FLOPPY_MFM | FLOPPY_MT | FLOPPY_SK | static_cast<uint8_t>(FloppyCommand::ReadData));
//            send_byte((head << 2) | slave());
//            send_byte(cylinder);
//            send_byte(head);
//            send_byte(sector);
//            send_byte(SECTORS_PER_CYLINDER >> 8); // Yikes!
//            send_byte(((sector + 1) >= SECTORS_PER_CYLINDER) ? SECTORS_PER_CYLINDER : sector + 1);
//            send_byte(0x1b); // GPL3 value. The Datasheet doesn't really specify the values for this properly...
//            send_byte(0xff);
//
//            x86::enable_irq();
//
//            wait_for_irq(); // TODO: See if there was a lockup here via some "timeout counter"
//            m_interrupted = false;
//
//            // Flush FIFO
//            // Let's check the value of Status Register 1 to ensure that
//            // the command executed correctly
//            uint8_t cmd_st0 = read_byte();
//            if ((cmd_st0 & 0xc0) != 0) {
////                klog() << "fdc: read failed with error code (st0) 0x" << String::write("%x", cmd_st0 >> 6);
//                m_lock.unlock();
//                return false;
//            }
//
//            uint8_t cmd_st1 = read_byte();
//            if (cmd_st1 != 0) {
////                klog() << "fdc: read failed with error code (st1) 0x" << String::write("%x", cmd_st1);
//                m_lock.unlock();
//                return false;
//            }
//
//            read_byte();
//            uint8_t cyl = read_byte();
//            read_byte();
//            read_byte();
//            read_byte();
//
//            if (cyl != cylinder) {
//#ifdef FLOPPY_DEBUG
//                klog() << "fdc: cyl != cylinder (cyl = " << cyl << " cylinder = " << cylinder << ")! Retrying...";
//#endif
//                continue;
//            }
//
//            // Let the controller know we handled the interrupt
//            send_byte(FloppyCommand::SenseInterrupt);
//            uint8_t st0 = read_byte();
//            uint8_t pcn = read_byte();
//            static_cast<void>(st0);
//            static_cast<void>(pcn);
//
//            memcpy(outbuf, PTR<void>(m_dma_buffer_page->phy_addr), 512 * count);
//
//            m_lock.unlock();
//            return true;
//        }
//
//#ifdef FLOPPY_DEBUG
//        klog() << "fdc: out of read attempts (check your hardware maybe!?)";
//#endif
//        m_lock.unlock();
//        return false;
//    }
//
//    bool FloppyDisk::write_sectors_with_dma(uint16_t lba, uint16_t count, const uint8_t *inbuf) {
//        m_lock.lock();// Acquire lock
//#ifdef FLOPPY_DEBUG
//        klog() << "fdc: write_sectors_with_dma lba = " << lba << " count = " << count;
//#endif
//
//        motor_enable(slave() ? 1 : 0); // Should I bother casting this?!
//        write_ccr(0);
//        recalibrate(); // Recalibrate the drive
//
//        if (!seek(lba)) {
////            klog() << "fdc: failed to seek to lba = " << lba << "!";
//            m_lock.unlock();
//            return false;
//        }
//
//        // We have to wait for about 300ms for the drive to spin up, because of
//        // the inertia of the motor and diskette.
//        // TODO: Fix this abomination please!
//        //u32 start = PIT::seconds_since_boot();
//        //while(start < PIT::seconds_since_boot() + 1)
//        //    ;
//
//        x86::disable_irq();
//
//        outb(0xA, FLOPPY_DMA_CHANNEL | 0x4); // Channel 2 SEL, MASK_ON = 1
//        outb(0x0B, 0x5A);                    // Begin DMA, Single Transfer, Increment, Auto, RAM -> FDC, Channel 2
//        outb(0xA, 0x2);                      // Unmask channel 2. The transfer will now begin
//
//        uint16_t cylinder = lba2cylinder(lba);
//        uint16_t head = lba2head(lba);
//        uint16_t sector = lba2sector(lba);
////
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: addr = 0x" << String::write("%x", lba * BYTES_PER_SECTOR) << " c = " << cylinder << " h = " << head << " s = " << sector;
////#endif
//
//        for (int i = 0; i < 3; i++) {
//            // Now actually send the command to the drive. This is a big one!
//            send_byte(FLOPPY_MFM | FLOPPY_MT | static_cast<uint8_t>(FloppyCommand::WriteData));
//            send_byte(head << 2 | slave());
//            send_byte(cylinder);
//            send_byte(head);
//            send_byte(sector);
//            send_byte(SECTORS_PER_CYLINDER >> 8); // Yikes!
//            send_byte((sector + 1) >= SECTORS_PER_CYLINDER ? SECTORS_PER_CYLINDER : sector + 1);
//            send_byte(0x1b); // GPL3 value. The Datasheet doesn't really specify the values for this properly...
//            send_byte(0xff);
//
//            x86::enable_irq();
//
//            wait_for_irq(); // TODO: See if there was a lockup here via some "timeout counter"
//            m_interrupted = false;
//
//            // Flush FIFO
//            uint8_t cmd_st0 = read_byte();
//            if ((cmd_st0 & 0xc0) != 0) {
////                klog() << "fdc: write failed! Error code 0x" << String::write("%x", cmd_st0 >> 6);
//                m_lock.unlock();
//                return false;
//            }
//
//            uint8_t cmd_st1 = read_byte();
//            if (cmd_st1 != 0) {
////                klog() << "fdc: write failed with error code (st1) 0x" << String::write("%x", cmd_st1);
//                m_lock.unlock();
//                return false;
//            }
//
//            read_byte();
//            uint8_t cyl = read_byte();
//            read_byte();
//            read_byte();
//            read_byte();
//
//            if (cyl != cylinder) {
//#ifdef FLOPPY_DEBUG
//                klog() << "fdc: cyl != cylinder (cyl = " << cyl << " cylinder = " << cylinder << ")! Retrying...";
//#endif
//                continue;
//            }
//
//            // Let the controller know we handled the interrupt
//            send_byte(FloppyCommand::SenseInterrupt);
//            uint8_t st0 = read_byte();
//            uint8_t pcn = read_byte();
//            static_cast<void>(st0);
//            static_cast<void>(pcn);
//
//            memcpy(PTR<void>(m_dma_buffer_page->phy_addr), inbuf, 512 * count);
//            m_lock.unlock();
//            return true;
//        }
//
//#ifdef FLOPPY_DEBUG
//        klog() << "fdc: out of read attempts (check your hardware maybe!?)";
//#endif
//        m_lock.unlock();
//        return false;
//    }
//
//
//    bool FloppyDisk::wait_for_irq() const {
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: Waiting for interrupt...";
////#endif
//
//        while (!m_interrupted) {
////            scheduler.schedule();
//        }
//
////        memory_barrier();
//        return true;
//    }
//
//    void FloppyDisk::handle_irq(x86::intr_frame_t &intr_frame) {
//        // The only thing we need to do is acknowledge the IRQ happened
//        m_interrupted = true;
//    }
//
//
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: Received IRQ!";
////#endif
//}
//
//void FloppyDisk::send_byte(uint8_t value) const {
//    for (int i = 0; i < 1024; i++) {
//        if (read_msr() & FLOPPY_MSR_RQM) {
//            outb(m_io_base_addr + FLOPPY_FIFO, value);
//            return;
//        }
//    }
////
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: FIFO write timed out!";
////#endif
//}
//
//void FloppyDisk::send_byte(FloppyCommand value) const {
//    for (int i = 0; i < 1024; i++) {
//        if (read_msr() & FLOPPY_MSR_RQM) {
//            outb(m_io_base_addr + FLOPPY_FIFO, static_cast<uint8_t>(value));
//            return;
//        }
//    }
////
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: FIFO write timed out!";
////#endif
//}
//
//uint8_t FloppyDisk::read_byte() const {
//    for (int i = 0; i < 1024; i++) {
//        if (read_msr() & (FLOPPY_MSR_RQM | FLOPPY_MSR_DIO)) {
//            return inb(m_io_base_addr + FLOPPY_FIFO);
//        }
//    }
////
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: FIFO read timed out!";
////#endif
//
//    return 0xff;
//}
//
//void FloppyDisk::write_dor(uint8_t value) const {
//    outb(m_io_base_addr + FLOPPY_DOR, value);
//}
//
//void FloppyDisk::write_ccr(uint8_t value) const {
//    outb(m_io_base_addr + FLOPPY_CCR, value);
//}
//
//uint8_t FloppyDisk::read_msr() const {
//    return inb(m_io_base_addr + FLOPPY_MSR);
//}
//
//void FloppyDisk::motor_enable(bool slave) const {
//    uint8_t val = slave ? 0x2D : 0x1C;
//    write_dor(val);
//}
//
//bool FloppyDisk::is_busy() const {
//    return read_msr() & FLOPPY_MSR;
//}
//
//bool FloppyDisk::recalibrate() {
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: recalibrating drive...";
////#endif
//
//    uint8_t slave = slave();
//    motor_enable(slave);
//
//    for (int i = 0; i < 16; i++) {
//        send_byte(FloppyCommand::Recalibrate);
//        send_byte(slave);
//        wait_for_irq();
//        m_interrupted = false;
//
//        send_byte(FloppyCommand::SenseInterrupt);
//        uint8_t st0 = read_byte();
//        uint8_t pcn = read_byte();
//        static_cast<void>(st0);
//
//        if (pcn == 0)
//            return true;
//    }
////
////#ifdef FLOPPY_DEBUG
////        klog() << "fdc: failed to calibrate drive (check your hardware!)";
////#endif
//    return false;
//}
//
//bool FloppyDisk::seek(uint16_t lba) {
//    uint8_t head = lba2head(lba) & 0x01;
//    uint8_t cylinder = lba2cylinder(lba) & 0xff;
//    uint8_t slave = slave();
//
//    // First, we need to enable the correct drive motor
//    motor_enable(slave);
//#ifdef FLOPPY_DEBUG
//    klog() << "fdc: seeking to cylinder " << cylinder << " on side " << head << " on drive " << slave;
//#endif
//
//    // Try at most 5 times to seek to the desired cylinder
//    for (int attempt = 0; attempt < 5; attempt++) {
//        send_byte(FloppyCommand::Seek);
//        send_byte((head << 2) | slave);
//        send_byte(cylinder);
//        wait_for_irq();
//        m_interrupted = false;
//
//        send_byte(FloppyCommand::SenseInterrupt);
//        uint8_t st0 = read_byte();
//        uint8_t pcn = read_byte();
//
//        if ((st0 >> 5) != 1 || pcn != cylinder || (st0 & 0x01)) {
//#ifdef FLOPPY_DEBUG
//            klog() << "fdc: failed to seek to cylinder " << cylinder << " on attempt " << attempt << "!";
//#endif
//            continue;
//        }
//
//        return true;
//    }
//
////        klog() << "fdc: failed to seek after 3 attempts! Aborting...";
//    return false;
//}
//
//// This is following Intel's datasheet for the 82077, page 41
//void FloppyDisk::initialize() {
//#ifdef FLOPPY_DEBUG
//    klog() << "fdc: m_io_base = 0x" << String::write("%x", m_io_base_addr) << " IRQn = " << IRQ_FLOPPY_DRIVE;
//#endif
//    x86::enable_irq();
//
//    // Get the version of the Floppy Disk Controller
//    send_byte(FloppyCommand::Version);
//    m_controller_version = read_byte();
////        klog() << "fdc: Version = 0x" << String::write("%x", m_controller_version);
//
//    // Reset
//    write_dor(0);
//    write_dor(FLOPPY_DOR_RESET | FLOPPY_DOR_DMAGATE);
//
//    write_ccr(0);
//    wait_for_irq();
//    m_interrupted = false;
//
//    // "If (and only if) drive polling mode is turned on, send 4 Sense Interrupt commands (required). "
//    // Sorry OSDev, but the Intel Manual states otherwise. This ALWAYS needs to be performed.
//    for (int i = 0; i < 4; i++) {
//        send_byte(FloppyCommand::SenseInterrupt);
//        uint8_t sr0 = read_byte();
//        uint8_t trk = read_byte();
//
////            klog() << "sr0 = 0x" << String::format("%x", sr0) << ", cyl = 0x" << String::write("%x", trk);
//    }
//
//    // This is hardcoded for a 3.5" floppy disk drive
//    send_byte(FloppyCommand::Specify);
//    send_byte(0x08); // (SRT << 4) | HUT
//    send_byte(0x0A); // (HLT << 1) | NDMA
//
//    // Allocate a buffer page for us to read into. This only needs to be one sector in size.
//    m_dma_buffer_page = mm.alloc_frame(); // TODO supervisor page?
//#ifdef FLOPPY_DEBUG
//    klog() << "fdc: allocated supervisor page at paddr 0x", String::write("%x", m_dma_buffer_page->paddr());
//#endif
//
//    // Now, let's initialise channel 2 of the DMA controller!
//    // This only needs to be done here, then we can just change the direction of
//    // the transfer
//    outb(0xA, FLOPPY_DMA_CHANNEL | 0x4); // Channel 2 SEL, MASK_ON = 1
//
//    outb(0xC, 0xFF); // Reset Master Flip Flop
//
//    // Set the buffer page address (the lower 16-bits)
//    outb(0x4, m_dma_buffer_page->phy_addr & 0xff);
//    outb(0x4, (m_dma_buffer_page->phy_addr >> 8) & 0xff);
//
//    outb(0xC, 0xFF); // Reset Master Flip Flop again
//
//    outb(0x05, (SECTORS_PER_CYLINDER * BYTES_PER_SECTOR) & 0xff);
//    outb(0x05, (SECTORS_PER_CYLINDER * BYTES_PER_SECTOR) >> 8);
//    outb(0x81, (m_dma_buffer_page->phy_addr >> 16) &
//                    0xff); // Supervisor page could be a 24-bit address, so set the External Page R/W register
//
//    outb(0xA, 0x2); // Unmask Channel 2
//
//#ifdef FLOPPY_DEBUG
//    klog() << "fdc: fd" << (slave() ? 1 : 0) << " initialised succesfully!";
//#endif
//    x86::enable_irq();
//}
