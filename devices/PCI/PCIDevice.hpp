//
// Created by Harold on 2020/5/15.
//

#ifndef OS_CPP_PCIDEVICE_HPP
#define OS_CPP_PCIDEVICE_HPP

#include <devices/PCI/PCIManager.hpp>

namespace kernel {
    class PCIDevice {
    private:
        uint8_t bus{};
        uint8_t slot{};
        uint8_t function{};
        PCIGeoAddress address;
    protected:
        uint32_t BAR0() {
            return pcim.BAR0(address);
        }

        uint32_t BAR1() {
            return pcim.BAR1(address);
        }
    };
}

#endif //OS_CPP_PCIDEVICE_HPP
