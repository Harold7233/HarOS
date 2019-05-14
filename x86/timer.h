#ifndef TIMER_H
#define TIMER_H
#include <common/common.h>
#define HZ 100
#define CLOCKS_PER_SEC
void init_timer();
extern u32int_t ticks;
#endif