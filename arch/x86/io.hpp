//
// Created by Harold on 2020/5/14.
//

#ifndef OS_CPP_IO_HPP
#define OS_CPP_IO_HPP

#include <sys/types.hpp>


inline uint8_t inb(uint16_t port) {
    uint8_t ret = 0;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

inline uint16_t inw(uint16_t port) {
    uint16_t ret = 0;
    asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

inline uint32_t inl(uint16_t port) {
    uint16_t ret = 0;
    asm volatile("inl %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

inline void outw(uint16_t port, uint16_t value) {
    asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}


inline void outl(uint16_t port, uint32_t value) {
    asm volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

inline void insl(uint16_t port, uint8_t *buffer, size_t count) {
    asm volatile("rep insl"
    : "+D"(buffer), "+c"(count)
    : "d"(port)
    : "memory");
}

inline void insw(uint16_t port, uint8_t *buffer, size_t count) {
    asm volatile("rep insl"
    : "+D"(buffer), "+c"(count)
    : "d"(port)
    : "memory");
}

inline void outsw(uint16_t port, uint8_t *buffer, size_t count) {
    asm volatile("rep outsw"
    : "+S"(buffer), "+c"(count)
    : "d"(port));
}


#endif //OS_CPP_IO_HPP
