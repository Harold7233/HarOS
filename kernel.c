#include "multiboot.h"
#include <common/common.h>
#include <x86/gdt.h>
#include <x86/idt.h>
#include <x86/timer.h>
#include <libc/string.h>
#include <common/dev.h>
#include <x86/interrupt.h>
#include <mm/paging.h>
#include <mm/heap.h>
#include <task/task.h>
#include <task/sched.h>
#include <kernel/syscall.h>
int kernel_main(multiboot_info_t *);
void page_fault(intr_regs_t);
void general_protection_fault(intr_regs_t regs);
extern multiboot_info_t *mboot_ptr;
//开启分页映射后的内核栈（0号进程的内核栈）
u8int_t kernel_stack[THREAD_INIT_STACK_SIZE];
int ttt(void *args)
{
    while (1)
    {
        printk("B");
        // schedule();
    }
}
void keyboard_cb(intr_registers){
    u8int_t scan_code = inb(0x60);
    printk("Key pressed: %d\n", scan_code);
}


int syscall_fork()
{
    int retval;
    asm volatile(
        "mov %%esp, %%ebx \n\t"
        "mov $1, %%eax \n\t"
        "int $0x80 \n\t"
        : "=&a"(retval));
    return retval;
}

int kernel_main(multiboot_info_t *mboot_ptr)
{

    init_gdt();
    init_idt();
    vga_clear();

    register_interrupt_handler(14, page_fault);
    register_interrupt_handler(128, syscall_dispathcer);
    register_interrupt_handler(33, keyboard_cb);
    // register_interrupt_handler(13, general_protection_fault);
    init_frames(mboot_ptr);
    init_paging();
    init_kernel_heap();
    
    init_tasking();
    init_timer();
    printk("Kernel Stack Top: 0x%08X\n", (u32int_t)&kernel_stack + THREAD_INIT_STACK_SIZE);
    kernel_thread(&ttt, NULL);
    while (1)
    {
        printk("A");
        // schedule();
    }
    halt();
    return 0;
}

__attribute__((section(".init.data"))) page_directory_t temp_dir __attribute__((aligned(4096)));
__attribute__((section(".init.data"))) page_table_t temp_tables[9] __attribute__((aligned(4096)));
__attribute__((section(".init.text"))) void init_paging_temp()
{

    temp_dir.tables[0] = (u32int_t)(&temp_tables[0]) | PAGE_PRESENT;
    u32int_t i = 0;

    /*恒等映射0-4M*/
    while (i < 1024)
    {
        temp_tables[0].pages[i] = (i << 12) | PAGE_PRESENT | PAGE_RW;
        ++i;
    }

    u32int_t addr = KERNEL_OFFSET;
    i = 0;
    /*临时映射0xC0000000-0xC0000000+32M到0x00000000-0x00000000+32M*/
    while (addr < KERNEL_OFFSET + 32 * 1024 * 1024)
    {
        page_table_t *table = &temp_tables[i / 1024 + 1];
        temp_dir.tables[PDE_INDEX(addr)] = (u32int_t)table | PAGE_PRESENT;
        for (int j = 0; j < 1024; ++j, ++i)
        {
            table->pages[PTE_INDEX(addr)] = (addr - KERNEL_OFFSET) | PAGE_PRESENT | PAGE_RW;
            addr += PAGE_SIZE;
        }
    }

    // 设置临时页表
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(&temp_dir));
    u32int_t cr0;
    // // BochsBreak();
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0"
                 :
                 : "r"(cr0));

    asm volatile("mov %0, %%esp\n"
                 "xor %%ebp, %%ebp"
                 :
                 : "r"((u32int_t)kernel_stack + THREAD_INIT_STACK_SIZE));
    kernel_main(mboot_ptr);
}

void page_fault(intr_regs_t regs)
{
    u32int_t addr;
    asm volatile("mov %%cr2, %0"
                 : "=r"(addr));
    u8int_t present = regs.error_code & 0x1;
    u8int_t write = (regs.error_code & 0x2) >> 1;
    u8int_t user_mode = (regs.error_code & 0x4) >> 2;
    u8int_t corrupted = (regs.error_code & 0x8) >> 3;
    printk("eip: 0x%08X\n", regs.eip);
    char msg[256];
    sprintf(msg, "Page fault at 0x%08X\nPresent: %d\nWrite: %d\nUser mode: %d\nEntry corrupted: %d\n",
            addr, present, write, user_mode, corrupted);
    panic_detail(msg, 0, 0, TEXT_FORE_COLOR_RED);
}

void general_protection_fault(intr_regs_t regs)
{
    char msg[256];
    sprintf(msg, "General Protection Falut at 0x%08X\n", regs.eip);
    panic_detail(msg, 0, 0, TEXT_FORE_COLOR_RED);
}
