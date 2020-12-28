//
// Created by Harold on 2020/5/9.
//

#ifndef OS_CPP_FRAME_HPP
#define OS_CPP_FRAME_HPP

#include <libcpp/list.hpp>

namespace kernel {
    class FrameChunk;

    class Frame {

        friend class BuddyManager;

        FrameChunk *chunk{};
    public:
        uint32_t phy_addr{};
    };

    class FrameChunk {
        friend class BuddyManager;

        ListNode free_list_node{};
        uint32_t start_frame_index{};
        uint8_t order;

    public:
        void *addr() {
            return reinterpret_cast<void *>(start_frame_index << 12U);
        }

        FrameChunk(uint32_t start_frame_index, uint8_t order)
                : free_list_node(), start_frame_index(start_frame_index), order(order) {

        }
    };
}
#endif //OS_CPP_FRAME_HPP
