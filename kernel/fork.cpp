//
// Created by Harold on 2020/4/26.
//
#include <sys/asm.hpp>
#include <kernel/fork.hpp>
#include <kernel/tasking/Scheduler.hpp>
#include <kernel/tasking/Task.hpp>
#include <sys/gcc.hpp>
#include <libcpp/cstring.hpp>
#include <arch/x86/gdt.hpp>
#include <kernel/memory/MemoryManager.hpp>
#include <kernel/memory/new.hpp>
extern tss_t tss_entry;
namespace {
    uint32_t cur_pid = 0;

    void NAKED kernel_task_wrapper() {
        asm volatile ("add $8, %esp\n"// 内核态中断不push ss和esp，跳过8字节
                      "call *4(%esp)\n"
                      "1:\n"
                      "jmp 1\n");
    }

    void NAKED new_user_task_entry() {
        asm volatile ("movl 8(%%esp), %%eax\n"
                      "movl %%eax, %0\n"
                      "xchg %%bx, %%bx\n"
                      "movl %%edx, %%cr3\n"
                      "iret":"=m"(tss_entry.eflags):"d"(VA(cur_task.pg_dir)));
    }

    void NAKED user_task_wrapper() {
        asm volatile ( "call *4(%esp)\n"
                       "1:\n"
                       "jmp 1\n");
    }

    void copy_kernel_page_table(page_directory_t *user_dir) {
        for (size_t i = PAGE_TABLE_INDEX(KERNEL_VADDR_OFFSET); i <= PAGE_TABLE_INDEX(0xFFFFFFFF); ++i) {
            user_dir->tables[i] = kernel_pg_dir->tables[i];
        }
    }

}
namespace kernel {
    void new_kernel_task_entry();

    void ktask_create(const char *name, int (*func)(void *), void *args) {
        auto *new_task = new Task(name, ++cur_pid);
        if (!cur_task.children) {
            cur_task.children = new List<Task, &Task::siblings_node>;
        }
        *reinterpret_cast<decltype(&func)>(new_task->stack_base() - sizeof(func)) = func;
        *reinterpret_cast<void **>(new_task->stack_base() - sizeof(func) - sizeof(args)) = args;
        void *frm_addr =
                new_task->stack_base() - sizeof(intr_frame_t::original_intr_frame_t) - sizeof(func) - sizeof(args);
        auto &frm = *reinterpret_cast<intr_frame_t::original_intr_frame_t *>(frm_addr);
        frm.eflags = 0x286;
        frm.cs = KERNEL_CS_SELECTOR;
        frm.ss = KERNEL_DS_SELECTOR;
        frm.eip = U32(&kernel_task_wrapper);
        new_task->k_esp = U32(frm_addr);
        new_task->sched_eip = U32(&new_kernel_task_entry);
        scheduler.activate_task(new_task);
        cur_task.children->append(*new_task);
        new_task->parent = &cur_task;
    }

    void utask_create(const char *name, int (*func)(void *), void *args) {
        auto *new_task = new Task(name, ++cur_pid);
        if (!cur_task.children) {
            cur_task.children = new List<Task, &Task::siblings_node>;
        }
        *reinterpret_cast<decltype(&func)>(new_task->stack_base() - sizeof(func)) = func;
        *reinterpret_cast<void **>(new_task->stack_base() - sizeof(func) - sizeof(args)) = args;
        void *context_addr =
                new_task->stack_base() - sizeof(intr_frame_t::original_intr_frame_t) - sizeof(func) - sizeof(args);
        auto &context = *reinterpret_cast<intr_frame_t::original_intr_frame_t *>(context_addr);
        context.eflags = 0x86;
        context.cs = USER_CS_SELECTOR | 0x3;
        context.ss = USER_DS_SELECTOR | 0x3;
        context.eip = U32(&user_task_wrapper);
        new_task->k_esp = U32(context_addr);
        context.user_esp = KERNEL_VADDR_OFFSET - 1;
        new_task->sched_eip = U32(&new_user_task_entry);
        scheduler.activate_task(new_task);
        cur_task.children->append(*new_task);
        tss_entry.esp0 = new_task->k_esp;
        tss_entry.ss0 = KERNEL_DS_SELECTOR;
        tss_entry.trap = 0;
        new_task->parent = &cur_task;
        new_task->pg_dir = new(mm.kmalloc_aligned(sizeof(page_directory_t), PAGE_SIZE))page_directory_t;
        copy_kernel_page_table(new_task->pg_dir);
    }
}

