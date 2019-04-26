#include <kernel/syscall.h>
#include <task/task.h>
static syscall_func_t syscall_table[MAX_SYSCALL_COUNT]={
    NULL
};

void syscall_dispathcer(intr_regs_t regs)
{
    if (regs.eax > MAX_SYSCALL_COUNT)
    {
        return;
    }
    syscall_func_t target = syscall_table[regs.eax];
    if (!target)
    {
        return;
    }
    int ret;

    asm volatile(" \ 
     push %1; \ 
     push %2; \ 
     push %3; \ 
     push %4; \ 
     push %5; \
     call *%6; \ 
     pop %%ebx; \ 
     pop %%ebx; \ 
     pop %%ebx; \ 
     pop %%ebx; \ 
     pop %%ebx; \ 
   "
                 : "=a"(ret)
                 : "r"(regs.edi), "r"(regs.esi), "r"(regs.edx), "r"(regs.ecx), "r"(regs.ebx), "r"(target));
    regs.eax = ret;
    printk("ret: %d\n", ret);
}

void register_syscall(u32int_t number, syscall_func_t fn)
{
    syscall_table[number] = fn;
}