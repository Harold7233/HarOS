#include <task/sched.h>
#include <task/task.h>
void schedule()
{
  preempt_disable();
  if (ready_queue)
  {
    task_t *prev = current_task;
    if (current_task->next)
    {
      // printk("Switching from %d to %d\n", prev->pid, current_task->next->pid);
      current_task = current_task->next;
      switch_to(prev, current_task, NULL);
    }
    else
    {
      //  printk("Switching from %d to %d\n", prev->pid, ready_queue->pid);
      current_task = ready_queue;
      switch_to(prev, current_task, NULL);
    }
  }
  preempt_enable();
}

void switch_to(task_t *prev, task_t *next, task_t *last)
{

  unsigned long ebx, ecx, edx, esi, edi;
  asm volatile("pushfl\n\t"                /* save flags 保存ebp、flags寄存器到旧线程的内核栈中*/
               "pushl %%ebp\n\t"           /* save    EBP   */
               "movl %%esp,%[prev_sp]\n\t" /* 将旧线程esp保存到context */
               "movl %[next_sp],%%esp\n\t" /* 切换至新线程的esp  */
               "movl $1f,%[prev_ip]\n\t"   /* save EIP 将该线程恢复执行时的下条地址保存到旧线程的context*/
               "pushl %[next_ip]\n\t"      /* restore EIP 将新线程的eip压入到新线程的内核栈中 */
               "ret \n\t"

               //  "jmp __switch_to\n"         /* regparm call  */
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