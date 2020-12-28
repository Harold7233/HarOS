//
// Created by Harold on 2020/4/26.
//

#ifndef OS_CPP_FORK_HPP
#define OS_CPP_FORK_HPP

#include <kernel/tasking/Scheduler.hpp>


namespace kernel {
    int kfork();

    void ktask_create(const char *name, int (*func)(void *), void *args = nullptr);

    void utask_create(const char *name, int (*func)(void *), void *args = nullptr);
}
#endif //OS_CPP_FORK_HPP
