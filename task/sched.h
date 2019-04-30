#ifndef SCHED_H
#define SCHED_H
#include <common/common.h>
#include <task/task.h>
#include <kernel/lock.h>
#include <x86/timer.h>
#define NICE_TO_TICKS(nice) ((20 - (nice)) >> 2 + 1)

#define SCHED_RR 1
#define SCHED_CFS 2

#define MAX_RT_PRIO 16
#define MAX_PRIO (MAX_RT_PRIO + 16)
#define MIN_TIMESLICE		( 10 * HZ / 1000)
#define MAX_TIMESLICE		(200 * HZ / 1000)
#define NEW_TIMESLICE(p) (MIN_TIMESLICE + \
	((MAX_TIMESLICE - MIN_TIMESLICE) * (MAX_PRIO-1-(p)->priority)/(MAX_PRIO - 1)))

#define SET_NEED_RESCHED(x)  \
    do                       \
    {                        \
        x->need_resched = 1; \
    } while (0)
#define RT_TASK(x) ((x)->priority <= MAX_RT_PRIO)
typedef struct prio_array
{
    u32int_t nr_active;
    int bitmap; // 32位bitmap，代表32个优先级
    struct list_head queue[MAX_PRIO];
} prio_array_t;

typedef struct runqueue
{
    spinlock_t lock;
    unsigned long nr_running, nr_switches, expired_timestamp,
        nr_uninterruptible;
    task_t *curr, *idle;
    struct mm_struct *prev_mm;
    prio_array_t *active, *expired, arrays[2];
} runqueue_t;

#endif