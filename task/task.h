#ifndef TASK_H
#define TASK_H
#include <common/common.h>
#include <common/vga.h>
#include <common/dev.h>
#include <mm/paging.h>
#define TSS_GDT_INDEX 5
#define THREAD_INIT_STACK_SIZE (PAGE_SIZE * 4)

#define preempt_count() (current_task->preempt_count)

#define inc_preempt_count()    \
    do                         \
    {                          \
        if (current_task)      \
            preempt_count()++; \
    } while (0)

#define dec_preempt_count()    \
    do                         \
    {                          \
        if (current_task)      \
            preempt_count()--; \
    } while (0)

#define preempt_disable()    \
    do                       \
    {                        \
        inc_preempt_count(); \
    } while (0)

#define preempt_enable_no_resched() \
    do                              \
    {                               \
        dec_preempt_count();        \
    } while (0)

#define preempt_enable()             \
    do                               \
    {                                \
        preempt_enable_no_resched(); \
    } while (0)

typedef int (*kthread_func)(void *);
typedef enum task_state
{
    TASK_UNKNOWN = 0,
    TASK_RUNNABLE = 1,
    TASK_SLEEPING = 2,

} task_state_t;

typedef struct context
{
    u32int_t esp;
    u32int_t ebp;
    u32int_t ebx;
    u32int_t ecx;
    u32int_t edx;
    u32int_t eip;
    u32int_t esi;
    u32int_t edi;
    u32int_t eflags;
} context_t;

typedef struct mm_struct
{
    page_directory_t *dir;
} mm_t;

union thread_info {
    struct pcb *task;
    u8int_t kernel_stack[PAGE_SIZE];
};
typedef struct prio_array prio_array_t;
typedef struct task_struct
{
    u32int_t pid;
    task_state_t state;
    void *stack;
    context_t context;
    mm_t *mm;
    u8int_t preempt_count;
    u8int_t need_resched;
    s8int_t nice;
    s8int_t priority;
    u32int_t time_slice;
    struct list_head run_list;
    prio_array_t *array;
} task_t;

// A struct describing a Task State Segment.
typedef struct tss_entry_struct
{
    u32int_t prev_tss;
    u32int_t esp0;
    u32int_t ss0;
    u32int_t esp1;
    u32int_t ss1;
    u32int_t esp2;
    u32int_t ss2;
    u32int_t cr3;
    u32int_t eip;
    u32int_t eflags;
    u32int_t eax;
    u32int_t ecx;
    u32int_t edx;
    u32int_t ebx;
    u32int_t esp;
    u32int_t ebp;
    u32int_t esi;
    u32int_t edi;
    u32int_t es;  // The value to load into ES when we change to kernel mode.
    u32int_t cs;  // The value to load into CS when we change to kernel mode.
    u32int_t ss;  // The value to load into SS when we change to kernel mode.
    u32int_t ds;  // The value to load into DS when we change to kernel mode.
    u32int_t fs;  // The value to load into FS when we change to kernel mode.
    u32int_t gs;  // The value to load into GS when we change to kernel mode.
    u32int_t ldt; // Unused...
    u16int_t trap;
    u16int_t iomap_base;
} __attribute__((packed)) tss_t;
void init_tasking();
void try_to_schedule();
void schedule();
void kernel_thread(kthread_func func, void *args, u32int_t priority);
void init_tss(u16int_t, u16int_t);
void switch_to_user_mode();
int fork();
extern task_t *current_task;
extern task_t *ready_queue;
#endif