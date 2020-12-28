//
// Created by Harold on 2020/4/30.
//

#ifndef OS_CPP_SEMAPHORE_HPP
#define OS_CPP_SEMAPHORE_HPP

#include <sys/asm.hpp>
#include <kernel/wait_queue.hpp>

namespace kernel {
    class Semaphore {
    protected:
        int val{};
        int min_val{};
        int max_val{};
        WaitQueue wait_queue;
    public:
        explicit Semaphore(int max_val, int min_val = INT_MIN) : val(max_val), min_val(min_val), max_val(max_val) {}

        virtual int wait();

        virtual int notify();
    };

}


#endif //OS_CPP_SEMAPHORE_HPP
