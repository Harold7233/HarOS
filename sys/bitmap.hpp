#ifndef SYS_BITMAP_H
#define SYS_BITMAP_H

#include <sys/types.hpp>

inline void clear_bit(int nr, volatile uint32_t *addr) {
    asm volatile("btrl %1,%0" : "=m"((*(volatile long *) addr)) : "Ir"(nr));
}

inline void set_bit(int nr, volatile uint32_t *addr) {
    asm volatile("btsl %1,%0" : "=m"((*(volatile long *) addr)) : "Ir"(nr));
}

inline uint32_t find_first_bit(uint32_t word) {
    asm volatile("bsfl %1,%0" : "=r"(word) : "rm"(word));
    return word;
}

#endif