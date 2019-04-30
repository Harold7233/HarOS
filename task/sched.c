#include <task/sched.h>
#include <kernel/bitmap.h>
#define STARVATION_LIMIT 100
#define EXPIRED_STARVING(rq)                                \
  (STARVATION_LIMIT && ((rq)->expired_timestamp &&          \
                        (ticks - (rq)->expired_timestamp >= \
                         STARVATION_LIMIT * ((rq)->nr_running) + 1)))

runqueue_t rq_entity, *rq = &rq_entity;
static inline void dequeue_task(struct task_struct *p, prio_array_t *array)
{
  array->nr_active--;
  list_del(&p->run_list);
  if (list_empty(array->queue + p->priority))
    clear_bit(p->priority, array->bitmap);
}

static inline void enqueue_task(struct task_struct *p, prio_array_t *array)
{
  list_add_tail(&p->run_list, array->queue + p->priority);
  set_bit(p->priority, array->bitmap);
  array->nr_active++;
  p->array = array;
}

void scheduler_tick()
{
  task_t *t = current_task;

  if (t == rq->idle)
  {
    // TODO 空闲进程
  }

  if (t->array != rq->active)
  {
    SET_NEED_RESCHED(t);
    return;
  }
  spin_lock(&rq->lock);

  if (!--t->time_slice)
  {
    if (RT_TASK(t))
    {
      t->time_slice = NEW_TIMESLICE(t);
      dequeue_task(t, rq->active);
      if (EXPIRED_STARVING(rq))
      {
        // 如果至少有一个任务expire超过了限制，即使是实时任务也不应该继续霸占CPU
        enqueue_task(t, rq->expired);
      }
      else
      {
        // 实时任务直接放回active队列
        enqueue_task(t, rq->active);
      }
    }
    else
    {
      dequeue_task(t, rq->active);
      SET_NEED_RESCHED(t);
      t->time_slice = NEW_TIMESLICE(t);
      enqueue_task(t, rq->expired);
    }
    if (!rq->expired_timestamp)
    {
      rq->expired_timestamp = ticks; // 如果所有任务都过期了，记录当前ticks数量
    }
  }

  spin_unlock(&rq->lock);
}

static inline void deactivate_task(struct task_struct *p)
{
  rq->nr_running--;
  dequeue_task(p, p->array);
  p->array = NULL;
}

void schedule()
{

  preempt_disable();
  task_t *prev = current_task, *next;
  prio_array_t *array;
  if (ready_queue)
  {
    task_t *prev = current_task;
    deactivate_task(prev);
    if (!rq->nr_running)
    {
      next = rq->idle;
      rq->expired_timestamp = 0;
    }
    array = rq->active;
    if (unlikely(!array->nr_active))
    {
      /*
		 * 如果active为空，交换expired和active两个prio_array
		 */
      rq->active = rq->expired;
      rq->expired = array;
      array = rq->active;
      rq->expired_timestamp = 0;
    }

    u8int_t idx = find_first_bit(array->bitmap); // 找到优先级最高的非空队列
    list_head_t *queue = array->queue + idx;
    next = list_entry(queue->next, task_t, run_list); // 选取该队列的第一个任务执行
    switch_to(prev, next, NULL);
  }
  preempt_enable();
}

void switch_to(task_t *prev, task_t *next, task_t *last)
{

  unsigned long ebx, ecx, edx, esi, edi;
  asm volatile("pushfl\n\t"                /* 保存eflags寄存器到旧线程的内核栈中 */
               "pushl %%ebp\n\t"           /* 保存ebp */
               "movl %%esp,%[prev_sp]\n\t" /* 将旧线程esp保存到context */
               "movl %[next_sp],%%esp\n\t" /* 切换至新线程的esp */
               "movl $1f,%[prev_ip]\n\t"   /* 将该线程恢复执行时的下条地址保存到旧线程的context */
               "pushl %[next_ip]\n\t"      /* 将新线程的eip压入到新线程的内核栈中，以此ret */
               "ret \n\t"

               //  "jmp __switch_to\n"         /* regparm call */
               "1:\t"

               "popl %%ebp\n\t" /* 该线程恢复执行，恢复ebp*/
               "popfl\n"        /* 恢复flags寄存器*/

               /* output parameters */
               : [prev_sp] "=m"(prev->context.esp),
                 [prev_ip] "=m"(prev->context.eip),
                 "=a"(last),

                 /* clobbered output registers: */
                 "=b"(ebx), "=c"(ecx), "=d"(edx),
                 "=S"(esi), "=D"(edi)

               /* input parameters: */
               : [next_sp] "m"(next->context.esp),
                 [next_ip] "m"(next->context.eip)

               //    /* regparm parameters for __switch_to(): */
               //    [prev] "a"(prev),
               //    [next] "d"(next)

               : /* reloaded segment registers */
               "memory");
}