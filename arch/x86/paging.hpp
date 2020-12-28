#ifndef _PAGING_H
#define _PAGING_H

#include <sys/types.hpp>
#include <sys/gcc.hpp>

#define PAGE_TABLE_INDEX(addr) (static_cast<uint32_t>(addr) >> 22)
#define PAGE_INDEX(addr) ((static_cast<u32int_t>(addr) >> 12) & 0x3FF)
#define FRAME_INDEX(x) ((x) >> 12)
#define ALIGN_ADDR_R(addr) (reinterpret_cast<u32int_t>(addr) & PAGE_ALIGN_MASK)
#define PAGE_ALIGN_ADDR(addr) (static_cast<uint32_t>(addr) & PAGE_ALIGN_MASK)
#define ALIGN_ADDR(addr, alignment) (static_cast<uint32_t>(addr) & ~(alignment-1))
#define VA(x) (reinterpret_cast<uint32_t>(x)+ KERNEL_VADDR_OFFSET)
#define KERNEL_ELF_END_VADDR  U32(&kernel::kernel_image_end_vaddr)
#define PA(x) (reinterpret_cast<uint32_t>(x)- KERNEL_VADDR_OFFSET)
#define PAGE_IS_USER(x)

namespace kernel {

    constexpr uint32_t KERNEL_MAX_MEMORY = 1 << 30;
    constexpr uint8_t PAGE_PRESENT = 0x1;
    constexpr uint8_t PAGE_USER = 0x4;
    constexpr uint8_t PAGE_RW = 0x2;
    constexpr uint32_t PAGE_SIZE = 4096;
    constexpr uint32_t MAX_PAGE_COUNT = KERNEL_MAX_MEMORY / PAGE_SIZE;
    constexpr uint32_t PAGE_KERNEL = 0;
    constexpr uint32_t PAGE_ACCESSED = 0x8;
    constexpr uint32_t PAGE_DIRTY = 0x10;
    constexpr uint32_t PAGE_ALIGN_MASK = 0xFFFFF000;
    constexpr uint32_t KERNEL_VADDR_OFFSET = 0xC0000000;
    extern "C" {
    extern uint8_t kernel_image_end_vaddr;
    }
    /*页表的实体，10位所以有1024个页*/
    struct PACKED page_table_t {
        uint32_t pages[1024]{};
    };

    /*页目录，10位所以有1024个页表*/
    struct PACKED page_directory_t {
        uint32_t tables[1024]{};  // table的物理地址
//        page_table_t *v_addr[1024]; // table的虚拟地址
//        uint32_t physicalAddr;
    };
    extern page_directory_t *kernel_pg_dir;


    bool INLINE page_user(uint32_t page) {
        return page & PAGE_USER;
    }

    bool INLINE page_present(uint32_t page) {
        return page & PAGE_PRESENT;
    }
//    struct Page {
//        uint32_t present: 1;  // 页是否在内存中
//        uint32_t rw: 1;       // 1为可读写，0为只读
//        uint32_t user: 1;     // 0为特权，1为普通用户
//        uint32_t accessed: 1;
//        uint32_t dirty: 1;
//        uint32_t unused: 7;  // 保留，无意义
//        uint32_t frame: 20;  // 页框地址的高20位
//
//        Page(uint32_t val) : present(val & 0x1), rw((val & 0x2) >> 1), user((val & 0x4) >> 2),
//                             accessed((val & 0x8) >> 3),
//                             dirty((val & 0x10) >> 4), frame(val >> 12) {
//        }
//
//        Page &operator=(uint32_t &rhs) {
//
//        }
//
//        uint32_t u32() {
//            return *reinterpret_cast<uint32_t *>(this);
//        }
//
//        uint32_t operator*() {
//            return *reinterpret_cast<uint32_t *>(this);
//        }
//    };




}  // namespace memory
using namespace kernel;

extern "C" {
void init_paging();
}
#endif