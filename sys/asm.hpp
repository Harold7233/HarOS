#ifndef _COMMON_ASM_H
#define _COMMON_ASM_H

#include <sys/types.hpp>
//#include <sys/gcc.hpp>

#define cli() asm volatile("cli")
#define sti() asm volatile("sti")
#define bb asm volatile("xchg %bx, %bx")
#define hlt asm volatile("hlt")
#define loop() while(1);
#define loop_asm() asm volatile("loop:jmp loop")
namespace x86 {
}  // namespace x86
#endif