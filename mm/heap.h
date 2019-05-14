#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H
#include <common/common.h>
#include <mm/paging.h>
#define KERNEL_HEAP_STARTS_AT 0xF0000000
#define KERNEL_HEAP_HEADER_MAGIC 0x1C2A3F4E
#define KERNEL_HEAP_MIN_BLOCK_SIZE 0
#define KERNEL_HEAP_MIN_SIZE 0x00001000
#define KERNEL_HEAP_MAX_SIZE 0x8000000
#define KERNEL_HEAP_ALIGNMENT_MAGIC 0xACE234D
// #define BLOCK_START(header) ((void*)((u32int)header+sizeof(heap_header_t))
typedef struct heap_header
{
    u32int_t magic;           // 4B
    struct heap_header *prev; // 4B
    struct heap_header *next; // 4B
    u32int_t allocated : 1;   // 4B
    u32int_t size : 31
}__attribute__((packed)) heap_header_t ;

void init_kernel_heap();
void kfree(void *);
void *kmalloc(u32int_t);
void *kmalloc_aligned(u32int_t);
void print_blocks();
#endif
