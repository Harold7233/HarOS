//
// Created by Harold on 2020/5/9.
//

#include <kernel/memory/BuddyManager.hpp>
#include <libcpp/cmath.hpp>

namespace kernel {
    BuddyManager::BuddyManager(size_t frame_count) : frames(new Frame[frame_count]), frame_count(frame_count) {
        max_order = ceil_order(frame_count);
        order_map = new List<FrameChunk, &FrameChunk::free_list_node>[max_order + 1];
        uint32_t bits = 1U << max_order;
        bitmap_size = ceil_divide(bits, sizeof(uint32_t));
        bitmap = new uint32_t[bitmap_size];
        for (size_t i = 0; i < frame_count; ++i) {
            frames[i].phy_addr = i << 12;
        }
    }

    /* 用于在长度为2^(order-1)的连续页框用尽后，将一段长度为2^order的连续页框一分为二 */
    FrameChunk *BuddyManager::split_chunk(FrameChunk *frame) const {
        order_map[frame->order].remove(*frame);
        auto new_order = --frame->order;
        auto buddy_start = frame->start_frame_index + (1U << new_order);
        auto *new_chunk = new FrameChunk(buddy_start, new_order);
        frames[buddy_start].chunk = new_chunk;
        order_map[new_order].append(*frame);
        order_map[new_order].append(*new_chunk);
        return new_chunk;
    }

    FrameChunk *BuddyManager::alloc_frames(size_t count) {
        uint32_t order = ceil_order(count);
        if (!order_map[order].empty()) {
            FrameChunk &chuck = order_map[order].pop();
            if (order == max_order) {
                return &chuck;
            }
            auto b_index = bit_index(chuck.start_frame_index, order);
            auto bit = get_bit(b_index);
            if (!bit) { // bit == 0
                set_bit(b_index);
            } else {
                clear_bit(b_index);
            }
        }
        FrameChunk *bigger_chunk{};
        do {
            // 向上寻找更长的区域
            auto &next = order_map[++order];
            if (!next.empty()) {
                bigger_chunk = &next.head();
                break;
            }
        } while (order <= max_order);
        if (!bigger_chunk) {
            // TODO fail？
            // 最长的空闲区域都无法满足要求，失败
            return nullptr;
        }
        do {
            bigger_chunk = split_chunk(bigger_chunk); // 不断分割，直到满足最小长度
        } while (bigger_chunk->order > order);
        set_bit(bit_index(bigger_chunk->start_frame_index, bigger_chunk->order));
        order_map[bigger_chunk->order].remove(*bigger_chunk);
        return bigger_chunk;
    }

    /* 释放一段连续的页框 */
    void BuddyManager::free_pages(FrameChunk *grp) {
        auto chunk_start = grp->start_frame_index;
        auto order = grp->order;
        assert(chunk_start % (1U << order) == 0); // 起始地址一定是2^order的整数倍
        FrameChunk *new_chunk = grp;
        while (order <= max_order) {
            auto cur_bit_index = bit_index(chunk_start, order);
            auto can_merge = get_bit(cur_bit_index);
            if (!can_merge) {
                set_bit(cur_bit_index);
                order_map[new_chunk->order].append(*new_chunk);
                frames[new_chunk->start_frame_index].chunk = new_chunk;
                break;
            }
            clear_bit(cur_bit_index);
            auto buddy_start = (cur_bit_index << (order + 1U)) - frame_count;
            FrameChunk *to_be_removed{};
            FrameChunk *buddy{};
            if (buddy_start == chunk_start) { // buddy的index和chunk_start相同，则真实的的buddy起始地址应该在chuck+2^order的位置
                buddy_start += 1U << order;
                to_be_removed = frames[buddy_start].chunk;
                buddy = to_be_removed;
                frames[buddy_start].chunk = nullptr;
            } else {
                to_be_removed = frames[chunk_start].chunk;
                frames[chunk_start].chunk = nullptr;
                new_chunk = frames[buddy_start].chunk;
                buddy = new_chunk;
            }
            order_map[buddy->order].remove(*buddy);
            delete to_be_removed;
            chunk_start = new_chunk->start_frame_index;
            ++new_chunk->order;
            ++order;
        }
    }

    /* 初始化一段物理地址，释放到空闲链表中 */
    void BuddyManager::free_range(uint32_t left, uint32_t right) {
        left = FRAME_INDEX(left);
        right = FRAME_INDEX(right);
        uint32_t order = 0;
        uint32_t right_bound = 0;
        while (left <= right) {
            /* floor(log2(left到right的长度))和 ceil(log2(left))取小的作为开始步长 */
            order = MIN(floor_order(right - left + 1),
                        ceil_order(left));
            while (left % (1U << order) != 0) {
                --order; // left一定是2^order的整数倍
            }
            right_bound = left + (1U << order) - 1;
            auto *chunk = new FrameChunk(left, order);
            free_pages(chunk);
            left = right_bound + 1;
        }
    }


}

