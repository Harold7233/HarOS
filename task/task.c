#include <task/task.h>
#include <mm/heap.h>
#include <x86/interrupt.h>
#include <x86/gdt.h>
#include <libc/string.h>
#include <task/sched.h>
#include <kernel/list.h>
task_t *ready_queue;
task_t *current_task;
u32int_t next_pid = 0;

void set_kernel_stack(u32int_t esp0);
extern u32int_t get_eip(); //asm
static void load_tss();
static void write_tss(s32int, u16int, u32int);
extern tss_t tss_entry;
extern u8int_t kernel_stack[];
extern runqueue_t *rq;
extern void ret_from_fork();
void init_tasking()
{
    cli();
    current_task = (task_t *)kmalloc(sizeof(task_t));
    ready_queue = current_task;
    current_task->pid = next_pid++;
    current_task->stack = (void *)&kernel_stack;
    current_task->context.esp = 0;
    current_task->context.ebp = 0;
    current_task->array = 0;

    rq->active = &rq->arrays[0];
    rq->expired = &rq->arrays[1];
    for (int i = 0; i < MAX_PRIO; ++i)
    {
        INIT_LIST_HEAD(&rq->active->queue[i]);
        INIT_LIST_HEAD(&rq->expired->queue[i]);
    }
    rq->idle = current_task;
    sti();
}

void do_exit()
{
    halt();
}
extern void ret_from_fork();
__asm__(".align 4\n"
        "ret_from_fork:\n\t"
        "add $4, %esp\n\t"
        "popa\n\t"
        "add $8, %esp\n\t"
        "iret");

extern void kernel_thread_helper(void);
__asm__(".align 4\n"
        "kernel_thread_helper:\n\t"
        "movl %edx,%eax\n\t"
        "pushl %edx\n\t"
        "call *%ebx\n\t"
        "pushl %eax\n\t"
        "call do_exit");

void kernel_thread(kthread_func func, void *args, u32int_t priority)
{
    asm volatile("cli");
    intr_regs_t regs;
    memset(&regs, 0, sizeof(intr_regs_t));
    regs.cs = KERNEL_CS_GDT_INDEX << 3;
    regs.ds = KERNEL_DS_GDT_INDEX << 3;
    regs.ss = KERNEL_DS_GDT_INDEX << 3;
    regs.useresp = 0;
    regs.eip = &kernel_thread_helper;
    regs.eflags = 0x286; // ?????
    regs.ebx = (u32int_t)func;
    regs.edx = (u32int_t)args;
    task_t *new_task = (task_t *)kmalloc(sizeof(task_t));
    new_task->pid = next_pid++;
    new_task->state = TASK_RUNNABLE;
    new_task->priority = priority;
    new_task->stack = kmalloc(THREAD_INIT_STACK_SIZE);
    new_task->context.eip = &ret_from_fork;
    regs.ebp = (u32int_t)new_task->stack + THREAD_INIT_STACK_SIZE;
    regs.esp = regs.ebp - sizeof(intr_regs_t);
    new_task->context.ebp = regs.ebp;
    new_task->context.esp = regs.esp;
    memcpy((void *)regs.esp, &regs, sizeof(intr_regs_t));
    asm("movl %[next_sp],%%esp"
        : [next_sp] "=m"(regs.esp));
    task_t *t = ready_queue;
    if (!t)
    {
        PANIC("No Root Process!");
    }
    wake_up_new_process(new_task);
    asm volatile("sti");
}
static void load_tss()
{
    asm volatile("\
        mov $0x2B, %ax\n\
        ltr %ax\
    ");
}

// void set_kernel_stack(u32int_t esp0)
// {
//     tss_entry.esp0 = esp0;
// }
void switch_to_user_mode()
{

    load_tss();
    tss_entry.esp0 = (u32int_t)&kernel_stack;
    // Set up a stack structure for switching to user mode.
    asm volatile("  \ 
     cli; \ 
     mov $0x23, %ax; \ 
     mov %ax, %ds; \ 
     mov %ax, %es; \ 
     mov %ax, %fs; \ 
     mov %ax, %gs; \ 
                   \ 
     mov %esp, %eax; \ 
     pushl $0x23; \ 
     pushl %eax; \ 
     pushf; \ 
     pushl $0x1B; \ 
     push $1f; \ 
     iret; \ 
   1:hlt; \ 
     ");
}