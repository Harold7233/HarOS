//
// Created by Harold on 2020/5/26.
//

#ifndef OS_CPP_MUTEX_HPP
#define OS_CPP_MUTEX_HPP

#include <sys/types.hpp>
#include <sys/asm.hpp>
#include <kernel/wait_queue.hpp>

namespace kernel {
    class Mutex {
        volatile Task *owner{};
        volatile bool val = true;
        WaitQueue wait_queue{};
    public:
        int acquire();
        int release();
    };


}


#endif //OS_CPP_MUTEX_HPP
