#ifndef _COMMON_H
#define _COMMON_H
typedef __UINT64_TYPE__ uint64_t;
typedef __INT64_TYPE__ int64_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __INT32_TYPE__ int32_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __INT16_TYPE__ int16_t;
typedef __UINT8_TYPE__ uint8_t;
typedef __INT8_TYPE__ int8_t;
typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef int32_t off_t;
typedef size_t align_val_t;
typedef uint64_t time_t;
#define INT_MAX 0x0FFFFFFF
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 0xFFFFFFFF
#define MAX(x, y) ((x) >= (y) ? x : y)
#define MIN(x, y) ((x) <= (y) ? x : y)
#define va_start(v, l)    __builtin_va_start(v,l)
#define va_end(v)    __builtin_va_end(v)
#define va_arg(v, l)    __builtin_va_arg(v,l)
typedef __builtin_va_list va_list;
#define bb asm volatile("xchg %bx, %bx")
#define KB(x) (x*(1U<<10))
#define MB(x) (x*(1U<<20))
#define GB(x) (x*(1U<<30))
#define U32(x) ((uint32_t)x)

#endif