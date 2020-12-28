//
// Created by Harold on 2020/5/14.
//

#ifndef OS_CPP_PCI_HPP
#define OS_CPP_PCI_HPP

#include <sys/types.hpp>

namespace kernel {
    constexpr uint16_t PCI_CONFIG_ADDR = 0xCF8;
    constexpr uint16_t PCI_CONFIG_DATA = 0xCFC;
    constexpr uint16_t PCI_NON_EXISTENT = 0xFFFF;
    constexpr uint8_t PCI_MULTIPLE_FUNCTION = 0x80;
    constexpr uint8_t PCI_HEADER_TYPE_BRIDGE = 0x80;
    enum class PCIConfigRegister : uint8_t {
        VENDOR_ID = 0x0,
        DEVICE_ID = 0x2,
        COMMAND = 0x4,
        STATUS = 0x6,
        REVISION_ID = 0x08,
        PROG_IF = 0x09,
        SUBCLASS = 0x0A,
        CLASS = 0x0B,
        CACHE_LINE_SIZE = 0x0C,
        LATENCY_TIMER = 0x0D,
        HEADER_TYPE = 0x0E,
        BIST = 0x0F,
        BAR0 = 0x10,
        BAR1 = 0x14,
        BAR2 = 0x18,
        BAR3 = 0x1C,
        BAR4 = 0x20,
        BAR5 = 0x24,
        CIS_PTR = 0x28,
        SUB_VENDOR_ID = 0x2C,
        SUB_SYS_ID = 0x2E,
        EXP_ROM_BASE_ADDR = 0x30,
        CAP_PTR = 0x34,
        INTR_LINE = 0x3C,
        INTR_PIN = 0x3D,
        MIN_GRANT = 0x3E,
        MAX_LATENCY = 0x3F,
        BRIDGE_VENDOR_ID = 0x01,
        BRIDGE_DEVICE_ID = 0x02,
        BRIDGE_COMMAND = 0x04,
        BRIDGE_STATUS = 0x06,
        BRIDGE_REVISION_ID = 0x08,
        BRIDGE_PROG_IF = 0x09,
        BRIDGE_SUBCLASS = 0x0A,
        BRIDGE_BIST = 0x0B,
        BRIDGE_BAR0 = 0x10,
        BRIDGE_BAR1 = 0x14,
        BRIDGE_PRIMARY_BUS_NUM = 0x18,
        BRIDGE_SECONDARY_BUS_NUM = 0x19,
        BRIDGE_SUBORDINATE_BUS_NUM = 0x1A,
        BRIDGE_SECONDARY_LATENCY_TIMER = 0x1B,
        BRIDGE_IO_BASE = 0x1C,
        BRIDGE_IO_LIMIT = 0x1D,
        BRIDGE_SECONDARY_STATUS = 0x1E,
        BRIDGE_MEMORY_BASE = 0x20,
        BRIDGE_MEMORY_LIMIT = 0x22,
        BRIDGE_PREFETCH_MEM_BASE = 0x24,
        BRIDGE_PREFETCH_MEM_LIMIT = 0x26,
        BRIDGE_PREFETCH_BASE_UPPER_32 = 0x28,
        BRIDGE_PREFETCH_LIMIT_UPPER_32 = 0x2C,
        BRIDGE_IO_BASE_UPPER_16 = 0x30,
        BRIDGE_IO_LIMIT_UPPER_16 = 0x32,
        BRIDGE_CAP_POINTER = 0x34,
        BRIDGE_EXP_ROM_BASE = 0x38,
        BRIDGE_INTR_LINE = 0x3C,
        BRIDGE_INTR_PIN = 0x3D,
        BRIDGE_BRIDGE_CONTROL = 0x3E
    };

    class PCIGeoAddress {
    private:
        uint8_t bus{0};
        uint8_t slot{0};
        uint8_t function{0};
    public:
        PCIConfigRegister reg{};

        uint32_t val() const {
            return 0x80000000 | bus << 16 | slot << 11 | function << 8 | static_cast<uint8_t>(reg) << 2;
        }

        uint32_t base_val() const {
            return 0x80000000 | bus << 16 | slot << 11 | function << 8;
        }

        explicit PCIGeoAddress(uint8_t bus = 0, uint8_t slot = 0, uint8_t function = 0,
                               PCIConfigRegister reg = PCIConfigRegister::VENDOR_ID)
                : bus(bus), slot(slot), function(function), reg(reg) {

        }

        explicit PCIGeoAddress(PCIConfigRegister reg) : reg(reg) {

        }

        bool target_found() const {
            return bus || slot || function;
        }
    };

    /* BAR */
    class BaseAddrReg {
    public:
        static uint32_t mem_base(uint32_t bar_val) {
            return bar_val & 0xFFFFFFF0;
        }

        static bool prefetch_available(uint32_t bar_val) {
            return bar_val & 0x8;
        }

        static bool memory_mapped(uint32_t bar_val) {
            return bar_val & 1;
        }

        static uint8_t type(uint32_t bar_val) {
            return (bar_val & 0b110) >> 1;
        }

        BaseAddrReg() = delete;
    };
}
#endif //OS_CPP_PCI_HPP
