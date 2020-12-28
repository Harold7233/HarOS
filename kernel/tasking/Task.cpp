//
// Created by Harold on 2020/4/14.
//

#include <kernel/tasking/Task.hpp>
#include <kernel/tasking/Scheduler.hpp>

namespace kernel {
    Task::Task(const char *name, pid_t pid) : pid(pid), name(name), stack(new uint8_t[KERNEL_STACK_SIZE]) {

    }

    void Task::activate() {
        scheduler.activate_task(this);
    }

    void Task::deactivate() {
        scheduler.deactivate_task(this);
    }

    Task::~Task() {
        delete pg_dir;
        delete children;
    }


}


