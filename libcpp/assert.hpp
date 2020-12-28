//
// Created by Harold on 2020/4/21.
//

#ifndef OS_CPP_ASSERT_HPP
#define OS_CPP_ASSERT_HPP
#include <kernel/stdio.hpp>
#define assert(x) \
    if (!(x)) PANIC(#x)

#endif  // OS_CPP_ASSERT_HPP
