//
// Created by Harold on 2020/5/6.
//

#ifndef OS_CPP_SIGNAL_HPP
#define OS_CPP_SIGNAL_HPP

#include <kernel/tasking/Task.hpp>

typedef int32_t sig_t;

typedef int (*sig_handler_t)();

class signal {
    pid_t pid;
    sig_t sig_val;
};

void register_sig_handler(sig_t sig, sig_handler_t handler);


#endif //OS_CPP_SIGNAL_HPP
