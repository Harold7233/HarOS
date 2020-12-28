//
// Created by Harold on 2020/4/14.
//

#ifndef OS_CPP_TASK_HPP
#define OS_CPP_TASK_HPP

#include <arch/x86/interrupt.hpp>
#include <kernel/kernel.hpp>
#include <libcpp/list.hpp>

namespace kernel {
    typedef uint32_t pid_t;


    class Task {
    public:
        pid_t pid{};
        const char *name{};
        Task *parent{};
        uint8_t *stack{};
        x86::intr_frame_t *context{};
        page_directory_t *pg_dir{};
        uint32_t k_esp{};
        uint32_t sched_eip{};
        ListNode rq_node{};
        ListNode siblings_node{};
        ListNode wait_queue_node{};
        List<Task, &Task::siblings_node> *children{};
        uint32_t time_slices{};
        bool need_reschedule{false};

        Task() {
            stack = new uint8_t[KERNEL_STACK_SIZE];
        }

        uint8_t *stack_base() const {
            return static_cast<uint8_t *>(stack + KERNEL_STACK_SIZE);
        }


        Task(const char *name, uint32_t pid);

        ~Task();

        void activate();

        void deactivate();

        void reschedule() {
            need_reschedule = true;
        }

    };

    enum class TaskState {
        TASK_RUNNING = 0,
        TASK_READY = 1,
        TASK_WAITING = 2,
        TASK_STOPPED = 3
    };


}
#endif //OS_CPP_TASK_HPP
