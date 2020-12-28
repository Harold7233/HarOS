//
// Created by Harold on 2020/5/5.
//

#ifndef OS_CPP_CMATH_HPP
#define OS_CPP_CMATH_HPP

#include <sys/types.hpp>


inline uint32_t ceil_divide(uint32_t dividend, uint32_t divisor) {
    return (dividend + divisor - 1) / divisor; // cool
    uint32_t r = dividend % divisor;
    if (r)
        return dividend / divisor + 1;
    return dividend / divisor;
}

inline uint32_t ceil_order(uint32_t num) {
    uint32_t order = 0;
    uint32_t original = num;
    while (num > 1) {
        num >>= 1U;
        ++order;
    }
    if (original & ~(1U << order)) {
        ++order;
    }
    return order;
}

inline uint32_t floor_order(uint32_t num) {
    uint32_t order = 0;
    while (num > 1) {
        num >>= 1U;
        ++order;
    }
    return order;
}

#endif //OS_CPP_CMATH_HPP
