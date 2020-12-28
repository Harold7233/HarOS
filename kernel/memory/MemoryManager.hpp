//
// Created by Harold on 2020/5/4.
//

#ifndef OS_CPP_MEMORYMANAGER_HPP
#define OS_CPP_MEMORYMANAGER_HPP

#include <arch/x86/paging.hpp>
#include <libcpp/list.hpp>

#define KMALLOC_POOL_START_ADDR  (KERNEL_ELF_END_VADDR + MB(1))
#define KMALLOC_POOL_SIZE  MB(128)
#define KMALLOC_POOL_END (KMALLOC_POOL_START_ADDR + KMALLOC_POOL_SIZE)
#define mm MemoryManager::instance()
struct multiboot_info;
namespace x86 {
    struct intr_frame_t;
}
namespace kernel {
    constexpr uint32_t VMALLOC_MEM_SIZE = MB(64);
    constexpr uint32_t ELASTIC_MEM_START = GB(4) - VMALLOC_MEM_SIZE;
    constexpr uint32_t FIXED_MEM_SIZE = GB(1) - VMALLOC_MEM_SIZE;

    class BuddyManager;

    class FrameChunk;

    class MemoryManager {
        static MemoryManager *mm_;

        static void init_heap();

        void page_fault_handler(x86::intr_frame_t intr_frame);

        void get_physical_mem_size(multiboot_info *);

        void init_kernel_paging();

        void init_frames(multiboot_info *);

        size_t mem_size{};
        page_directory_t *kernel_dir_{};
        page_table_t *kernel_tables_{};

        explicit MemoryManager(multiboot_info *);

    public:
        static MemoryManager &instance() {
            return *mm_;
        }

        static void init(multiboot_info *);

        FrameChunk *alloc_frames(size_t count);

        void free_frames(FrameChunk *);

        static void *kmalloc(size_t);

        static void kfree(void *);

        static void *kmalloc_aligned(size_t, size_t alignment);

        void flush_page_directory(page_directory_t *dir) {
            asm volatile ("movl %0, %%cr3"::"r"(PA(dir)));
        }

        BuddyManager *bm;
    };

//    extern MemoryManager *const mm;

}


#endif //OS_CPP_MEMORYMANAGER_HPP
