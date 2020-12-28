//
// Created by Harold on 2020/6/15.
//

#ifndef OS_CPP_ALLOCATOR_HPP
#define OS_CPP_ALLOCATOR_HPP

#include <kernel/memory/MemoryManager.hpp>

namespace kernel {
    template<typename T>
    class Allocator {
    private:
        alignas(T) uint8_t *data{nullptr};
    public:
        Allocator() = default;

        void construct() {

        }
    };

}


#endif //OS_CPP_ALLOCATOR_HPP
