#ifndef COMMON_HEADERS
#define COMMON_HEADERS
typedef unsigned int u32int_t;
typedef int s32int_t;
typedef unsigned short u16int_t;
typedef short s16int_t;
typedef unsigned char u8int_t;
typedef char s8int_t;
typedef unsigned int size_t;
#define INT_MAX 0x0FFFFFFF
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 0xFFFFFFFF
#define NULL 0
#define MAX(x, y) ((x) >= (y) ? x : y)
#define MIN(x, y) ((x) <= (y) ? x : y)
extern void *kernel_start_offset;
extern void *kernel_end_offset;

void outb(u16int_t port, u8int_t value);
u8int_t inb(u16int_t port);

void panic_detail(char *, char *, u32int_t, u8int_t);
#include <common/vga.h>
#define PANIC(msg) panic_detail(msg, __FILE__, __LINE__, TEXT_FORE_COLOR_RED)
#define bb asm volatile("xchg %bx, %bx");
#define halt()   \
    do           \
    {            \
        for (;;) \
            ;    \
    } while (0)
#endif