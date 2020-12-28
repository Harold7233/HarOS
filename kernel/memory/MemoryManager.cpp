//
// Created by Harold on 2020/5/4.
//
#include <kernel/memory/MemoryManager.hpp>
#include <kernel/memory/BuddyManager.hpp>
#include <boot/multiboot.h>
#include <kernel/stdio.hpp>
#include <libcpp/cmath.hpp>
#include <arch/x86/interrupt.hpp>
#include <sys/asm.hpp>

#define BLOCK_DATA_START(x) (U32(x) + sizeof(HeapBlockHeader))
#define BLOCK_END(x) (U32(x)+(x->data_size)+sizeof(HeapBlockHeader))
#define BLOCK_SIZE(x) (x->size - sizeof(HeapBlockHeader))
namespace {
    constexpr uint32_t KMALLOC_BLOCK_MAGIC = 0xCAFE123;
    struct HeapBlockHeader {
        uint32_t magic: 30;
        uint32_t aligned: 1;
        uint32_t allocated: 1;
        uint32_t data_size;
        HeapBlockHeader *next;
        HeapBlockHeader *prev;
    } *heap_head, *heap_tail;

    void merge_all_free() {

    }

    uint32_t align_block(const HeapBlockHeader *header, size_t size, size_t alignment) {
        uint32_t aligned_at = ALIGN_ADDR(BLOCK_DATA_START(header), alignment);
        if (aligned_at != BLOCK_DATA_START(header)) {
            // 如果数据区首地址没有对齐，则对齐
            aligned_at = ALIGN_ADDR(BLOCK_DATA_START(header) + alignment, alignment);
        }
        if (aligned_at >= BLOCK_END(header))
            return 0;
        uint32_t remainder = BLOCK_END(header) - aligned_at;
        if (remainder < size) {
            return 0;
        }
        return aligned_at + size;

    }

    inline HeapBlockHeader *init_header(HeapBlockHeader *header) {
        header->allocated = 0;
        header->aligned = 0;
        header->magic = KMALLOC_BLOCK_MAGIC;
        header->next = nullptr;
        header->prev = nullptr;
        header->data_size = 0;
        return header;
    }


    HeapBlockHeader *split_block(HeapBlockHeader *header, uint32_t split_at, uint32_t align_offset) {

        if (!align_offset) { //如果不需要对齐或者数据区已经对齐
            if (!header->next || header->next->allocated) {
                uint32_t remainder = BLOCK_END(header) - split_at;
                if (remainder > sizeof(HeapBlockHeader)) { //剩下的空间足够构造一个块
                    header->data_size = split_at - BLOCK_DATA_START(header);

                    auto *new_block = init_header(reinterpret_cast<HeapBlockHeader *>(split_at));
                    new_block->data_size = remainder - sizeof(HeapBlockHeader);
                    new_block->next = header->next;
                    if (new_block->next) {
                        new_block->next->prev = new_block;
                    }
                    new_block->prev = header;
                    header->next = new_block;
                }
                // 否则不分割，直接返回header
            } else { // 右侧有块且未被占用，直接合并进去
                uint32_t next_size = header->next->data_size;
                auto *next_next = header->next->next;
                uint32_t old_size = header->data_size;
                header->data_size = split_at - BLOCK_DATA_START(header);
                auto *new_next_header = init_header(reinterpret_cast<HeapBlockHeader *>(U32(split_at)));
                new_next_header->data_size = next_size + (old_size - header->data_size);
                new_next_header->next = next_next;
                new_next_header->prev = header;
            }
        } else { // 如果数据区没有对齐
            //先保存prev和next指针，后面移动操作可能会丢失
            uint32_t block_end = BLOCK_END(header);
            auto *prev = header->prev;
            auto *next = header->next;
            // header往后移动offset个字节
            auto *new_header = init_header(reinterpret_cast<HeapBlockHeader *>(U32(header) + align_offset));
            new_header->next = next;
            new_header->prev = prev;
            if (prev) { // 如果前面有块，直接合并到其尾部，不管是否被占用
                prev->data_size += align_offset;
                prev->next = new_header;
            } else if (align_offset > sizeof(HeapBlockHeader)) { // 如果前面没有块，但是空间足够构造一个块
                auto *new_prev_block = header; //新块的header和原来块是同一个，所以和heap_head的连接仍保持
                new_prev_block->allocated = 0;
                new_prev_block->next = new_header;
                new_header->prev = new_prev_block;
                new_prev_block->data_size = align_offset - sizeof(HeapBlockHeader);
            } else {
                heap_head = new_header; // FIXME 空间越来越小？
            }
            auto *old = header;
            header = new_header;
            /*剩余的空间，必须用用之前保存的block_end计算，因为new_header的data_size还没有赋值，不能用BLOCK_END宏*/
            uint32_t remainder = block_end - split_at;

            if (remainder > sizeof(HeapBlockHeader)) { //剩下的空间足够构造一个块
                new_header->data_size = split_at - BLOCK_DATA_START(new_header);
                auto *new_block = init_header(reinterpret_cast<HeapBlockHeader *>(split_at));
                new_block->data_size = remainder - sizeof(HeapBlockHeader);
                new_block->next = nullptr;
                new_block->prev = new_header;
                new_header->next = new_block;
            } else {
                new_header->data_size = block_end - U32(new_header);
            }
        }
        if (header == heap_tail && header->next) {
            heap_tail = header->next;
        }
        return header;
    }

    HeapBlockHeader *merge_blocks(HeapBlockHeader *left, HeapBlockHeader *right) {
        left->next = right->next;
        left->data_size += right->data_size + sizeof(HeapBlockHeader);
        if (left->next) {
            left->next->prev = left;
        }
        return left;
    }

    void *kmalloc_common(size_t size, uint32_t alignment) {

        bool retried = false;
        retry:
        auto *cur = heap_head;
        while (cur) {
            if (cur->allocated || cur->data_size < size) {
                if (cur->next == nullptr) {
                    heap_tail = cur;
                }
                cur = cur->next;
                continue;
            }
            uint32_t split_at = 0;
            if (alignment == 1) {
                split_at = BLOCK_DATA_START(cur) + size;
            } else {
                split_at = align_block(cur, size, alignment);
                if (split_at == 0) {
                    cur = cur->next;
                    continue;
                }
            }
            uint32_t align_offset = split_at - size - BLOCK_DATA_START(cur);
            auto *block = split_block(cur, split_at, align_offset);
            block->allocated = 1;
            const char *sizes[3] = {"B", "KB", "MB"};
            uint8_t idx = 0;
            uint16_t size_ = 0;
            if (block->data_size < 1024) {
                idx = 0;
                size_ = size;
            } else if (block->data_size >= 1024 && block->data_size < 1024 * 1024) {
                idx = 1;
                size_ = size / 1024;
            } else {
                idx = 2;
                size_ = size / 1024 / 1024;
            }
//            kprint("%d%s\t", size_, sizes[idx]);
//            kprint("%d%s\t", size, sizes[idx]);
            return reinterpret_cast<void *>(BLOCK_DATA_START(block));
        }
        // 合并所有连续空闲块再次尝试分配 TODO
        if (!retried) {
            merge_all_free();
            retried = true;
            goto retry;
        }
        return nullptr;
    }

//    void print_blocks() {
//        HeapBlockHeader *cur_ = heap_head;
//        while (cur_) {
//            kprint("Address: 0x%08X\t Allocated: %d\t Size: %d B\n", cur_, cur_->allocated, cur_->data_size);
//            cur_ = cur_->next;
//        }
//    }
}


namespace kernel {
    MemoryManager *MemoryManager::mm_{};

    MemoryManager::MemoryManager(multiboot_info *mboot_ptr) : kernel_dir_(new page_directory_t) {
        get_physical_mem_size(mboot_ptr);
        init_frames(mboot_ptr);
        init_kernel_paging();
    }

    void MemoryManager::init(multiboot_info *mboot_ptr) {
        init_heap();
        mm_ = new MemoryManager(mboot_ptr);
    }

    void *MemoryManager::kmalloc(size_t size) {
        if (size < 8) {
            return kmalloc_common(size, 1);
        }
        return kmalloc_common(size, 8);
    }

    void *MemoryManager::kmalloc_aligned(size_t size, size_t alignment) {
        return kmalloc_common(size, alignment);
    }

    void MemoryManager::kfree(void *addr) {
        auto *header = reinterpret_cast<HeapBlockHeader *>(U32(addr) - sizeof(HeapBlockHeader));
        auto *old_header = header;
        if (header->magic != KMALLOC_BLOCK_MAGIC) {
            PANIC("Attempting to free a block not allocated by kmalloc!");
        }
        header->allocated = 0;
        if (header->prev && !header->prev->allocated) {
            header = merge_blocks(header->prev, header);
        }
        if (header->next) {
            if (!header->next->allocated) {
                merge_blocks(header, header->next);
            }
        }
//        kprint("Freed block (size: %d Bytes) at %x\n", header->data_size, old_header);

    }

    void MemoryManager::get_physical_mem_size(multiboot_info *mboot_ptr) {
        uint32_t mmap_end = mboot_ptr->mmap_addr + mboot_ptr->mmap_length;
        auto *mmap = reinterpret_cast<memory_map_t *>(mboot_ptr->mmap_addr);
        uint32_t upper = 0;
        for (; U32(mmap) < mmap_end; ++mmap) {
            if (mmap->type != 1)
                continue;
            upper = mmap->base_addr_low + mmap->length_low;
        }
        mem_size = upper;
    }

    void MemoryManager::init_heap() {

        heap_head = init_header(reinterpret_cast<HeapBlockHeader *>(KMALLOC_POOL_START_ADDR));
        heap_head->data_size = KMALLOC_POOL_SIZE - sizeof(HeapBlockHeader);
    }

    void MemoryManager::init_kernel_paging() {
        const uint32_t start = KERNEL_VADDR_OFFSET;
        const uint32_t end = MIN(KERNEL_VADDR_OFFSET + mem_size, 0xFFFFFFFF);
        const uint32_t range = end - start;
        const uint32_t table_count = ceil_divide(range, MB(4));
        kernel_dir_ = static_cast<page_directory_t *>(kmalloc_aligned(sizeof(page_directory_t), PAGE_SIZE));
        kernel_tables_ = static_cast<page_table_t *>(kmalloc_aligned(sizeof(page_table_t) * table_count, PAGE_SIZE));

        for (size_t i = 0; i < table_count; ++i) {
            auto *table = &kernel_tables_[i];
            for (int j = 0; j < 1024; ++j) {
                table->pages[j] = ((j + i * 1024) * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
            }
            auto table_index = PAGE_TABLE_INDEX(KERNEL_VADDR_OFFSET + (i << 22));
            kernel_dir_->tables[table_index] = PA(table) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
        }
        flush_page_directory(kernel_dir_);
    }

    void MemoryManager::init_frames(multiboot_info *mboot_ptr) {
        uint32_t frame_count = mem_size / PAGE_SIZE;
        uint32_t mmap_end = mboot_ptr->mmap_addr + mboot_ptr->mmap_length;
        auto *mmap = reinterpret_cast<memory_map_t *>(mboot_ptr->mmap_addr);
        bm = new BuddyManager(frame_count);
        for (; U32(mmap) < mmap_end; ++mmap) {
            if (mmap->type != 1)
                continue;
            uint32_t upper = mmap->base_addr_low + mmap->length_low;
            upper = upper & PAGE_ALIGN_MASK;
            uint32_t base = MAX(mmap->base_addr_low, PA(KMALLOC_POOL_END)); // base必须大于heap结束的位置
            if (PAGE_ALIGN_ADDR(base) != base) {
                base = PAGE_ALIGN_ADDR((base + PAGE_SIZE)); //向上页对齐
            }
            bm->free_range(base, upper);
//            if (first_free_frame == 0) {
//                first_free_frame = FRAME_INDEX(base);
//            }
//            uint32_t &t = base;
//            while (t < upper_bound) {
//                t += PAGE_SIZE;
//            }
        }
    }

    void MemoryManager::page_fault_handler(x86::intr_frame_t intr_frame) {
//        uint32_t v_addr = pgf_addr();
//        uint32_t err = regs.error_code;
//        Task *task = cur_task();
//        if (!(err & PF_PROTECTION_NON_PRESENT)) {
//            if (task->pg_dir) {
//                frame_t *new_frame = alloc_frame();
//                map_page(task->pg_dir, v_addr, new_frame->phy_addr, PAGE_USER | PAGE_PRESENT);
//            } else {
//                // TODO KERNEL
//            }
//        } else {
//            PANIC("The page fault was caused by a protection violation\n");
//        }
//        bb;
//
////        kprint("Page Fault at %x", v_addr);
    }

    FrameChunk *MemoryManager::alloc_frames(size_t count) {
        return bm->alloc_frames(count);
    }

    void MemoryManager::free_frames(FrameChunk *chunk) {
        bm->free_pages(chunk);
    }

}

