//
// Created by Harold on 2020/5/26.
//

#include <kernel/locks/Mutex.hpp>
#include <kernel/tasking/Scheduler.hpp>
#include <sys/asm.hpp>
#include <arch/x86/asm.hpp>

namespace kernel {
    int Mutex::acquire() {
        local_irq_save();
        auto *cur = &cur_task;
        if (cur == owner) {
            local_irq_restore();
            return 0;
        }
        while (!val) { // 有可能被唤醒和被调度之间其他进程拿到了锁，所以被调度之后必须重新检测val的值，否则会2个进程同时lock成功！
            wait_queue.add(&cur_task);
            scheduler.schedule();
        }
        owner = cur;
        val = false;
        local_irq_restore();
        return 0;
//        assert(owner == nullptr && val);

    }

    int Mutex::release() {
        uint32_t intr_state;
        save_irq_and_disable(intr_state);
        wait_queue.wake_up_one();
        val = true;
        owner = nullptr;
        restore_irq(intr_state);
        return 0;
    }
}