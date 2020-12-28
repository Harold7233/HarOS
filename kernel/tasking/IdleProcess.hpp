//
// Created by Harold on 2020/5/13.
//

#ifndef OS_CPP_IDLEPROCESS_HPP
#define OS_CPP_IDLEPROCESS_HPP

#include <kernel/tasking/Task.hpp>

namespace kernel {
    class IdleProcess : Task {
    public:
        IdleProcess() = default;

        [[noreturn]]int run(void *args);
    };
}


#endif //OS_CPP_IDLEPROCESS_HPP
