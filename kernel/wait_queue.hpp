//
// Created by Harold on 2020/4/30.
//

#ifndef OS_CPP_WAIT_QUEUE_HPP
#define OS_CPP_WAIT_QUEUE_HPP

#include <libcpp/list.hpp>
#include <kernel/tasking/Task.hpp>

namespace kernel {

    class WaitQueue {
        List<Task, &Task::wait_queue_node> queue{};
    public:
        void add(Task *task);

        void wake_up_one();
    };
}

#endif //OS_CPP_WAIT_QUEUE_HPP
