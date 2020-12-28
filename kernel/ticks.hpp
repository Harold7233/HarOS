//
// Created by Harold on 2020/7/3.
//

#ifndef OS_CPP_ticks_HPP
#define OS_CPP_ticks_HPP

#include <sys/types.hpp>
#include <arch/x86/timer.hpp>

namespace kernel {
    extern volatile uint32_t ticks;

    constexpr uint32_t ms_to_ticks(uint32_t ms) {
        return ms * x86::FREQUENCY / 1000;
    }

    constexpr uint32_t sec_to_ticks(uint32_t seconds) {
        return seconds * x86::FREQUENCY;
    }

}
#endif //OS_CPP_ticks_HPP
