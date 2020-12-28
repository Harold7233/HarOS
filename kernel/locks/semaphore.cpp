//
// Created by Harold on 2020/4/30.
//


namespace kernel {
//    int Semaphore::wait() {
//        uint32_t intr_state;
//        x86::save_irq_and_disable(intr_state);
////        x86::disable_irq();
//        if (val == min_val) {
//            // TODO
//            PANIC("semaphore min");
//        } else if (--val >= 0) {
//            x86::restore_irq(intr_state);
//            return 0;
//        }
//        wait_queue.add(cur_task());
//        core_scheduler->schedule();
//        x86::restore_irq(intr_state);
//        return 0;
//    }
//
//    int Semaphore::notify() {
//        x86::disable_irq();
//        if (val == max_val) {
//            // TODO
//            PANIC("semaphore max");
//        } else if (++val > 0) {
//            x86::enable_irq();
//            return 0;
//        }
//        wait_queue.wake_up_one();
//        x86::enable_irq();
//        return 0;
//    }


}