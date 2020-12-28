//
// Created by Harold on 2020/5/14.
//

#ifndef OS_CPP_PCIMANAGER_HPP
#define OS_CPP_PCIMANAGER_HPP

#include <devices/PCI/PCI.hpp>
#include <kernel/stdio.hpp>
#include <libk/Callable.hpp>

#define pcim (PCIManager::instance())
namespace kernel {

    class PCIFindTarget {
        uint8_t class_code_{0xFF};
        uint8_t sub_class_code_{0xFF};

    public:
        PCIFindTarget(uint8_t class_code, uint8_t sub_class_code) : class_code_(class_code),
                                                                    sub_class_code_(sub_class_code) {

        }

        PCIFindTarget() = default;

        bool match(uint8_t class_code, uint8_t sub_class_code) const {
            return class_code_ == class_code && sub_class_code_ == sub_class_code;
        }
    };

    class PCIManager {
        friend class PCIDevice;

    private:
        static PCIManager *ins;
        PCIFindTarget find_target{};

        PCIGeoAddress enumerate_slots_in_bus(uint8_t bus);

        PCIGeoAddress enumerate_functions_in_slot(uint8_t bus, uint8_t slot);

        uint8_t read_cr_b(const PCIGeoAddress &, PCIConfigRegister);

        uint16_t read_cr_w(const PCIGeoAddress &, PCIConfigRegister);

        uint32_t read_cr_l(const PCIGeoAddress &address, PCIConfigRegister reg);

        void write_cr_b(const PCIGeoAddress &, PCIConfigRegister, uint8_t val);

        void write_cr_w(const PCIGeoAddress &, PCIConfigRegister, uint16_t val);

        void write_cr_l(const PCIGeoAddress &address, PCIConfigRegister reg, uint32_t val);


        uint32_t BAR0(PCIGeoAddress address) {
            return read_cr_l(address, PCIConfigRegister::BAR0);
        }

        uint32_t BAR1(PCIGeoAddress address) {
            return read_cr_l(address, PCIConfigRegister::BAR1);
        }

        void write_BAR0(const PCIGeoAddress &address, uint32_t bar_val) {
            write_cr_l(address, PCIConfigRegister::BAR0, bar_val);
        }

        PCIGeoAddress enumerate_devices() {
            int count = 0;
            if ((read_cr_b(PCIGeoAddress{}, PCIConfigRegister::HEADER_TYPE) & PCI_MULTIPLE_FUNCTION)) {
                // 多个PCI Host Controller
                count = 7;
            }
            for (int i = 0; i <= count; ++i) {
                PCIGeoAddress address(0, 0, i);
                if (read_cr_w(address, PCIConfigRegister::VENDOR_ID) == PCI_NON_EXISTENT)
                    continue;
                auto result = enumerate_slots_in_bus(i);
                if (result.target_found()) {
                    return result;
                }
            }
            return PCIGeoAddress();
        }

    public:
        static void init() {
            ins = new PCIManager;
        }

        static PCIManager &instance() {
            return *ins;
        }


        PCIGeoAddress find_device(PCIFindTarget target) {
            find_target = target;
            return enumerate_devices();
        }


    };
}

#endif //OS_CPP_PCIMANAGER_HPP
