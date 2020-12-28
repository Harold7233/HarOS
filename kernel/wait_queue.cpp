//
// Created by Harold on 2020/4/30.
//

#include <kernel/wait_queue.hpp>
#include <kernel/tasking/Scheduler.hpp>

namespace kernel {
    void WaitQueue::add(Task *task) {
        task->deactivate();
        queue.append(*task);
    }

    void WaitQueue::wake_up_one() {
        if (queue.empty()) {
            return;
        }
        auto &task = queue.left_pop();
        task.activate();
        cur_task.reschedule();
    }
}
