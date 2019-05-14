#ifndef WAIT_H
#define WAIT_H
#include <common/common.h>
#include <kernel/list.h>
#include <task/task.h>
#include <kernel/lock.h>
typedef struct wait_queue{
    list_head_t task_list;
    task_t *task;
} wait_queue_t;

typedef struct wait_queue_head{
    list_head_t task_list;
    spinlock_t lock;
} wait_queue_head_t;

#endif