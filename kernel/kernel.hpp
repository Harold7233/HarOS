#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H
#include <sys/types.hpp>
#include <arch/x86/paging.hpp>
namespace kernel {

constexpr uint32_t KERNEL_PERMANENT_MEMORY_SIZE = 0x002000000;
constexpr uint32_t KERNEL_STACK_SIZE = 8192;

}
#endif