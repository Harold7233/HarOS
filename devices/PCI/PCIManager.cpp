//
// Created by Harold on 2020/5/14.
//
#include <devices/PCI/PCIManager.hpp>
#include <arch/x86/io.hpp>


namespace kernel {
    PCIManager *PCIManager::ins{};

    uint8_t PCIManager::read_cr_b(const PCIGeoAddress &address, PCIConfigRegister reg) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC)); // 寄存器偏移地址必须是4的倍数
        return inb(PCI_CONFIG_DATA + (uint8_t(reg) & 0b11)); // 取reg的偏移地址在一个32位字中的偏移
    }

    uint16_t PCIManager::read_cr_w(const PCIGeoAddress &address, PCIConfigRegister reg) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC));
        return inw(PCI_CONFIG_DATA + (uint8_t(reg) & 0b10)); // 取reg的偏移地址在一个32位字中的偏移
    }

    uint32_t PCIManager::read_cr_l(const PCIGeoAddress &address, PCIConfigRegister reg) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC));
        return inl(PCI_CONFIG_DATA);
    }

    void PCIManager::write_cr_b(const PCIGeoAddress &address, PCIConfigRegister reg, uint8_t val) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC));
        outb(PCI_CONFIG_DATA + (uint8_t(reg) & 0b11), val);
    }

    void PCIManager::write_cr_w(const PCIGeoAddress &address, PCIConfigRegister reg, uint16_t val) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC));
        outw(PCI_CONFIG_DATA + (uint8_t(reg) & 0b10), val);
    }

    void PCIManager::write_cr_l(const PCIGeoAddress &address, PCIConfigRegister reg, uint32_t val) {
        outl(PCI_CONFIG_ADDR, address.base_val() | (uint8_t(reg) & 0xFC));
        outl(PCI_CONFIG_DATA, val);
    }


    PCIGeoAddress PCIManager::enumerate_slots_in_bus(uint8_t bus) {
        for (int i = 0; i < 32; ++i) {
            auto result = enumerate_functions_in_slot(bus, i);
            if (result.target_found()) {
                return result;
            }
        }
        return PCIGeoAddress();
    }

    PCIGeoAddress PCIManager::enumerate_functions_in_slot(uint8_t bus, uint8_t slot) {
        PCIGeoAddress address(bus, slot);
        if (read_cr_w(address, PCIConfigRegister::VENDOR_ID) == PCI_NON_EXISTENT)
            return PCIGeoAddress();
        int count = 1;
        if (read_cr_b(address, PCIConfigRegister::HEADER_TYPE) & PCI_MULTIPLE_FUNCTION)
            count = 8;
        for (int j = 0; j < count; ++j) {
            PCIGeoAddress addr(bus, slot, j);
            auto vendor_id = read_cr_w(addr, PCIConfigRegister::VENDOR_ID);
            if (vendor_id == PCI_NON_EXISTENT)
                continue;
            auto type = read_cr_b(addr, PCIConfigRegister::HEADER_TYPE);
            if (type & PCI_HEADER_TYPE_BRIDGE)
                continue;
            auto class_code = read_cr_b(addr, PCIConfigRegister::CLASS);
            auto sub_class = read_cr_b(addr, PCIConfigRegister::SUBCLASS);
            if (find_target.match(class_code, sub_class))
                return addr;
        }
        return PCIGeoAddress();
    }
}