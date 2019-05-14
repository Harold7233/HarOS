#ifndef BITMAP_H
#define BITMAP_H
#include <common/common.h>
static inline void clear_bit(int nr, volatile u32int_t * addr)
{
	__asm__ __volatile__(
		"btrl %1,%0"
		:"=m" ((*(volatile long *) addr))
		:"Ir" (nr));
}

static inline void set_bit(int nr, volatile u32int_t * addr)
{
	__asm__(
		"btsl %1,%0"
		:"=m" ((*(volatile long *) addr))
		:"Ir" (nr));
}

static inline u32int_t find_first_bit(u32int_t word)
{
	__asm__("bsfl %1,%0"
		:"=r" (word)
		:"rm" (word));
	return word;
}

#endif