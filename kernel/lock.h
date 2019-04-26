#ifndef LOCK_H
#define LOCK_H
#include <common/common.h>
#include <task/task.h>
typedef struct
{
    u32int_t dummy;
} spinlock_t;

#define spin_lock(lock)    \
    do                     \
    {                      \
        preempt_disable(); \
    } while (0)

#define spin_unlock(lock) \
    do                    \
    {                     \
        preempt_enable(); \
    } while (0)

#endif