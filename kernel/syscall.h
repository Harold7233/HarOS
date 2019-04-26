#ifndef SYSCALL_H
#define SYSCALL_H
#include <common/common.h>
#include <x86/interrupt.h>
#define MAX_SYSCALL_COUNT 16
typedef void (*syscall_func_t)(intr_regs_t);
void register_syscall(u32int_t number, syscall_func_t fn);
void syscall_dispathcer(intr_regs_t) ;
#endif