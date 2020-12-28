//
// Created by Harold on 2020/4/14.
//

#ifndef OS_CPP_SCHEDULER_HPP
#define OS_CPP_SCHEDULER_HPP

#include <kernel/tasking/Task.hpp>
#include <libcpp/list.hpp>
#include <libk/NonNull.hpp>

#define scheduler  kernel::Scheduler::instance()
#define cur_task scheduler.current_task()
namespace kernel {
    class Task;

    class Scheduler {
        static Scheduler *instance_;

        List<Task, &Task::rq_node> *rq{};
        List<Task, &Task::rq_node> *expired_q{};

        Scheduler();

        Task *pick_next();

    public:
        NonNull<Task> cur_;

        static Scheduler &instance() {
            return *instance_;
        }

        static void init();

        Task &current_task() {
            return *cur_;
        }

//        static Scheduler &instance() { return *scheduler; }
        void switch_to(Task *to, Task *from);

        void context_switch(Task *to, Task *from);

        void activate_task(Task *);

        void deactivate_task(Task *);

        void schedule();

        void tick();

    };

}  // namespace kernel
#endif  // OS_CPP_SCHEDULER_HPP
