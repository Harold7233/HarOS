//
// Created by Harold on 2020/5/9.
//

#ifndef OS_CPP_BUDDYMANAGER_HPP
#define OS_CPP_BUDDYMANAGER_HPP

#include <kernel/memory/Frame.hpp>
#include <libcpp/list.hpp>
#include <kernel/stdio.hpp>

namespace kernel {
    class BuddyManager {
    private:
        /*平分一个FrameChunk，返回右侧FrameChunk指针*/
        FrameChunk *split_chunk(FrameChunk *frame) const;

    public:
        Frame *frames;
        uint8_t max_order{};
        uint32_t *bitmap{};
        size_t bitmap_size{};
        size_t frame_count{};
        List<FrameChunk, &FrameChunk::free_list_node> *order_map{};

        BuddyManager(size_t total_frame);

        void free_range(uint32_t left, uint32_t right);

        FrameChunk *alloc_frames(size_t count);

        void free_pages(FrameChunk *grp);

        /*长度为2^order的块，对应的位图中的可合并指示位*/
        inline uint32_t bit_index(uint32_t frame_index, uint8_t order) const {
            return (frame_index + frame_count) >> (order + 1U);
        }

        bool get_bit(uint32_t index) const {
            return (bitmap[index / 32] & (1U << (index % 32)));
        }

        void set_bit(uint32_t index) {
            bitmap[index / 32] |= (1U << (index % 32));
        }

        void clear_bit(uint32_t index) {
            bitmap[index / 32] &= ~(1U << (index % 32));
        }

        void print() const {
            for (int j = 0; j <= max_order; ++j) {
                kprint("2^%d: ", j);
                for (auto &chunk:order_map[j]) {
                    kprint("%d,%d ", chunk.start_frame_index, chunk.start_frame_index + (1U << j) - 1);
                }
                kprint("\n");
            }
        }


    };
}

#endif //OS_CPP_BUDDYMANAGER_HPP
