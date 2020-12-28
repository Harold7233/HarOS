//
// Created by Harold on 2020/4/14.
//
#include <kernel/tasking/Scheduler.hpp>
#include <kernel/kernel.hpp>
#include <kernel/memory/MemoryManager.hpp>
#include <kernel/tasking/Task.hpp>
#include <kernel/stdio.hpp>
#include <arch/x86/gdt.hpp>
#include <sys/asm.hpp>
#include <libcpp/cstring.hpp>
#include <kernel/fork.hpp>
#include <sys/gcc.hpp>
#include <kernel/locks/semaphore.hpp>
#include <devices/FloppyDisk.hpp>
#include <kernel/memory/BuddyManager.hpp>
#include <kernel/tasking/IdleProcess.hpp>

using namespace x86;
namespace {
    Task *dummy_task;

//    [[noreturn]] int user_test(void *args) {
//        loop();
//    }

    [[noreturn]]int test2(void *args) {
        kprint("2nd Task Created!\n");
        loop();
    }

    [[noreturn]]void idle_task_func() {
        delete dummy_task;
        kprint("INIT SUCCESS!\n");
        IdleProcess().run(nullptr);
//        mm.bm->write();
        loop();
        while (true) {
            kprint("1");
        }
    }


}

namespace kernel {
    Scheduler *Scheduler::instance_{};

    void NAKED new_kernel_task_entry() {
        asm volatile ("iret\n"
                      ""
                      ""
                      ""
                      ""
                      ""
                      "");
    }

    Scheduler::Scheduler() : rq(new List<Task, &Task::rq_node>), expired_q(new List<Task, &Task::rq_node>) {

    }

    void Scheduler::init() {
        instance_ = new Scheduler;
        auto *idle_task = new Task("init task", 0);
        void *frm_addr = idle_task->stack_base() - sizeof(intr_frame_t::original_intr_frame_t);
        memset(frm_addr, 0, sizeof(intr_frame_t::original_intr_frame_t));
        auto &frm = *reinterpret_cast<intr_frame_t::original_intr_frame_t *>(frm_addr);
        frm.eflags = 0x286;
        frm.cs = KERNEL_CS_SELECTOR;
        frm.ss = KERNEL_DS_SELECTOR;
        frm.eip = U32(&idle_task_func);
        frm.user_esp = U32(idle_task->stack_base());
        idle_task->k_esp = U32(frm_addr);
        idle_task->sched_eip = U32(&new_kernel_task_entry);
        instance_->activate_task(idle_task);
        auto *dummy_task = new Task("dummy task", 0);
        instance_->cur_ = dummy_task;
        instance_->cur_ = dummy_task;
        dummy_task->k_esp = U32(dummy_task->stack_base());
        dummy_task->need_reschedule = true;
        dummy_task->time_slices = INT_MAX;
    }


    void Scheduler::switch_to(Task *to, Task *from) {
        asm volatile(
        "pushal\n"
        "pushfl\n"
        "movl $switched_back_normal, %[from_eip]\n"
        "movl %%esp, %[from_esp] \n"
        "movl %[to_esp], %%esp\n"
        "pushl %[to_eip]\n"
        "ret\n"
        "switched_back_normal:\n"
        "popfl\n"
        "popal\n"
        : [ from_esp ] "=m"(from->k_esp), [from_eip] "=m"(from->sched_eip)
        : [ to_esp ] "m"(to->k_esp), [to_eip] "m"(to->sched_eip));
    }

    void Scheduler::context_switch(Task *to, Task *from) {
        switch_to(to, from);
    }


    void Scheduler::schedule() {
        Task *next_task = pick_next();
        cur_->need_reschedule = false;
        if (cur_ == next_task) {
            return;
        }
        Task *prev = cur_;
        cur_ = next_task;
        context_switch(next_task, prev);

    }


    Task *Scheduler::pick_next() {
        if (rq->empty()) {
            if (expired_q->empty())
                return nullptr;
            auto *t = expired_q;
            expired_q = rq;
            rq = t;
            for (auto &task: *rq) {
                task.time_slices = 5;
            }
        }
        if (rq->size() == 1)
            return &rq->head();
        return &rq->head();
    }

    void Scheduler::tick() {
        if (rq->empty()) {
            return;
        } else {
            assert(cur_->time_slices > 0);
            if (!--cur_->time_slices) {
                rq->remove(*cur_);
                expired_q->append(*cur_);
                cur_->need_reschedule = true;
//            if (rq->empty()) {
//                auto *t = expired_q;
//                expired_q = rq;
//                rq = t;
//                for (auto &task: *rq) {
//                    task.time_slices = 5; // TODO
//                }
//            }
            }
//            else {
//                rq->remove(*cur_);
//                rq->append(*cur_);
//            }
//        }
        }
    }

    void Scheduler::activate_task(Task *task) {
        task->time_slices = 5;
        rq->append(*task);
    }

    void Scheduler::deactivate_task(Task *task) {
        rq->remove(*task);
//    expired_q->remove(*task);
    }


}  // namespace kernel

