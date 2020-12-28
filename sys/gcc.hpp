//
// Created by Harold on 2020/4/24.
//

#ifndef OS_CPP_GCC_HPP
#define OS_CPP_GCC_HPP

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define INLINE __attribute__((always_inline)) inline
#define C_LINKAGE extern "C"
#define INIT_TEXT __attribute__((section(".init.text")))
#define INIT_DATA __attribute__((section(".init.data")))
#define UNUSED __attribute__((unused))
#define PAGE_ALIGNED __attribute__((aligned(4096)))
#define barrier() __asm__ __volatile__("":::"memory")
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
#endif //OS_CPP_GCC_HPP
