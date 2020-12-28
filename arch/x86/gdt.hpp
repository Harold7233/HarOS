#ifndef _GDT_H
#define _GDT_H

#include <sys/types.hpp>
#include <sys/gcc.hpp>

namespace x86 {
    constexpr uint8_t GDT_TABLE_SIZE = 6;
    constexpr uint8_t KERNEL_CS_GDT_INDEX = 1;
    constexpr uint32_t GDT_SEGMENT_LEN = 0xFFFFFFFF;
    constexpr uint8_t KERNEL_DS_GDT_INDEX = 2;
    constexpr uint8_t USER_CS_GDT_INDEX = 3;
    constexpr uint8_t USER_DS_GDT_INDEX = 4;
    constexpr uint8_t KERNEL_CS_SELECTOR = KERNEL_CS_GDT_INDEX << 3;
    constexpr uint8_t KERNEL_DS_SELECTOR = KERNEL_DS_GDT_INDEX << 3;
    constexpr uint8_t USER_CS_SELECTOR = USER_CS_GDT_INDEX << 3;
    constexpr uint8_t USER_DS_SELECTOR = USER_DS_GDT_INDEX << 3;
    constexpr uint8_t TSS_ENTRY_INDEX = 5;
    constexpr uint8_t TSS_ENTRY_SELECTOR = TSS_ENTRY_INDEX << 3;
    struct PACKED gdt_entry_t {
        uint16_t limit_low;   // limit 0-15位
        uint16_t base_low;    // base 0-15位
        uint8_t base_middle;  // base 16-23位
        uint8_t accessed: 1;
        uint8_t rw: 1;
        uint8_t dc: 1;
        uint8_t executable: 1;
        uint8_t not_tss: 1;
        uint8_t dpl: 2;
        uint8_t present: 1;
        uint8_t limit_middle: 4;  // LIMIT 16-19位，AVL，0，0，G位
        uint8_t rsvd: 2;
        uint8_t protected_mode: 1;
        uint8_t page_granularity: 1;
        uint8_t base_high;  // base 24-31位
    };

    struct PACKED tss_t {
        uint32_t prev_tss{};
        uint32_t esp0{};
        uint32_t ss0{};
        uint32_t esp1{};
        uint32_t ss1{};
        uint32_t esp2{};
        uint32_t ss2{};
        uint32_t cr3{};
        uint32_t eip{};
        uint32_t eflags{};
        uint32_t eax{};
        uint32_t ecx{};
        uint32_t edx{};
        uint32_t ebx{};
        uint32_t esp{};
        uint32_t ebp{};
        uint32_t esi{};
        uint32_t edi{};
        uint32_t es{};
        uint32_t cs{};
        uint32_t ss{};
        uint32_t ds{};
        uint32_t fs{};
        uint32_t gs{};
        uint32_t ldt{};
        uint16_t trap{};
        uint16_t iomap_base{};
    };

// 读入GDTR寄存器的内容，共48位
    struct PACKED gdt_ptr_t {
        uint16_t limit;  // gdt表长度-1
        uint32_t base;   // gdt表基址
    };

    void init_gdt();

    void set_gdt_entry(uint8_t index, uint32_t base, uint32_t limit,
                       bool rw, bool executable, uint8_t dpl, bool is_tss = false);

    void init_tss();

}  // namespace x86
using namespace x86;
#endif