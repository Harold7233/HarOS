#include <mm/heap.h>
#include <common/dev.h>
#include <libc/string.h>
#define block_start(x) ((u32int_t)x + sizeof(heap_header_t))
#define block_size(x) (x->size - sizeof(heap_header_t))
static heap_header_t *split_block(heap_header_t *, u32int_t);
static heap_header_t *expand_heap(u32int_t, heap_header_t *);
static void *base_kmalloc(u32int_t, u8int_t);
static inline void init_header(heap_header_t *);
static heap_header_t *merge_blocks(heap_header_t *, heap_header_t *);
heap_header_t *heap_head;
static u32int_t heap_end;
void init_kernel_heap()
{

    heap_head = (heap_header_t *)KERNEL_HEAP_STARTS_AT;
    heap_end = (u32int_t)heap_head + KERNEL_HEAP_MIN_SIZE;
    u32int_t cur = KERNEL_HEAP_STARTS_AT;
    while (cur <= KERNEL_HEAP_STARTS_AT + KERNEL_HEAP_MIN_SIZE)
    {
        alloc_map(&kernel_page_dir, cur, PAGE_PRESENT | PAGE_RW);
        cur += PAGE_SIZE;
    }
    init_header(heap_head);
    heap_head->size = KERNEL_HEAP_MIN_SIZE;
}
void *kmalloc(u32int_t size)
{
    return base_kmalloc(size, 0);
}
void *kmalloc_aligned(u32int_t size)
{
    return base_kmalloc(size, 1);
}
static void *base_kmalloc(u32int_t size, u8int_t aligned)
{
   
    heap_header_t *cur = heap_head;
     
    heap_header_t *prev = heap_head;
    u32int_t addr_to_return = block_start(cur);
try_to_kmalloc:
    while (cur)
    {
        
        if (cur->allocated || cur->size < size + sizeof(heap_header_t))
        {
            prev = cur;
            cur = cur->next;
            continue;
        }
        if (aligned)
        {
            u32int_t new_block_start = (block_start(cur) + PAGE_SIZE - 1) & PAGE_ALIGN_MASK;
            if (new_block_start - block_start(cur) < sizeof(u32int_t) * 2 || new_block_start + size > (u32int_t)cur + cur->size)
            {
                prev = cur;
                cur = cur->next;
                continue;
            }
            size += new_block_start - block_start(cur);

            addr_to_return = new_block_start;
            u32int_t *magic = new_block_start - 4;
            *magic = KERNEL_HEAP_ALIGNMENT_MAGIC;
            *(magic - 4) = cur;
        }
        else
        {
            addr_to_return = (u32int_t)cur + sizeof(heap_header_t);
        }

        heap_header_t *other_half = split_block(cur, size);
        if (other_half)
        {
            other_half->next = cur->next;
            if (cur->next)
                cur->next->prev = other_half;
            cur->next = other_half;
            other_half->prev = cur;
        }
        printk("Allocated %dB at 0x%08X (Block at 0x%08X)\n", size, cur, addr_to_return);
        cur->allocated = 1;
        return (void *)addr_to_return;
    }

    heap_header_t *last_header = expand_heap((size + 2 * sizeof(heap_header_t) + PAGE_SIZE - 1) / PAGE_SIZE, prev);
    cur = last_header;
    goto try_to_kmalloc;
}

static inline void init_header(heap_header_t *header)
{
    header->allocated = 0;
    header->magic = KERNEL_HEAP_HEADER_MAGIC;
    header->next = 0;
    header->prev = 0;
    header->size = 0;
}

void kfree(void *allocated)
{
    heap_header_t *header;
    if (*(u32int_t *)((u32int_t)allocated - 4) == KERNEL_HEAP_ALIGNMENT_MAGIC)
    {
        header = (heap_header_t *)(*(u32int_t *)((u32int_t)allocated - 8));
    }
    else
    {
        header = (heap_header_t *)((u32int_t)allocated - sizeof(heap_header_t));
    }

    if (header->magic != KERNEL_HEAP_HEADER_MAGIC)
    {
        PANIC("Attempting to free a block not allocated by kmalloc!");
    }
    header->allocated = 0;
    if (header->prev)
    {
        if (!header->prev->allocated)
        {
            header = merge_blocks(header->prev, header);
        }
    }
    if (header->next)
    {
        if (!header->next->allocated)
        {
            merge_blocks(header, header->next);
        }
    }
}

static heap_header_t *split_block(heap_header_t *header, u32int_t requested_size)
{
    u32int_t remainder = header->size - requested_size - 2 * sizeof(heap_header_t);
    if (remainder < KERNEL_HEAP_MIN_BLOCK_SIZE)
    {
        return NULL; // 分割失败
    }

    header->size = requested_size + sizeof(heap_header_t);
    heap_header_t *new_header = (heap_header_t *)((u32int_t)header + header->size);
    init_header(new_header);
    new_header->size = remainder + sizeof(heap_header_t);
    return new_header;
}

// static heap_header_t *split_block_aligned(heap_header_t *header, u32int_t requested_size)
// {
//     u32int_t unaligned_block_start = (u32int_t)header + sizeof(heap_header_t);
//     u32int_t aligned_block_start = (unaligned_block_start + PAGE_SIZE - 1) & PAGE_ALIGN_MASK;
//     if (header->size+sizeof(heap_header_t)<= aligned_block_start){
//         return NULL;
//     }
//     if (aligned_block_start - unaligned_block_start - sizeof(heap_header_t) < KERNEL_HEAP_MIN_BLOCK_SIZE){
//         aligned_block_start += PAGE_SIZE;

//     }
//     if (aligned_block_start)
// }

static heap_header_t *merge_blocks(heap_header_t *left, heap_header_t *right)
{
    left->size = left->size + right->size;
    left->next = right->next;
    if (left->next)
        left->next->prev = left;
    memset((void *)((u32int_t)left + sizeof(heap_header_t)), 0, left->size - sizeof(heap_header_t));
    return left;
}

static heap_header_t *expand_heap(u32int_t page_count, heap_header_t *last_header)
{
    printk("page_count: %d\n", page_count);
    u32int_t temp_end = heap_end;
    if (heap_end + page_count * PAGE_SIZE > KERNEL_HEAP_STARTS_AT + KERNEL_HEAP_MAX_SIZE)
    {
        PANIC("Not enough space for heap expanding!");
    }
    while (heap_end < temp_end + page_count * PAGE_SIZE)
    {
        alloc_map(&kernel_page_dir, heap_end, PAGE_PRESENT | PAGE_RW);
        heap_end += PAGE_SIZE;
    }
    if (last_header->allocated)
    {
        heap_header_t *new_last_header = (u32int_t)last_header + last_header->size;
        init_header(new_last_header);
        new_last_header->size = heap_end - (u32int_t)new_last_header;
        new_last_header->prev = last_header;
        last_header->next = new_last_header;
        printk("Expanded with new header\n");
        return new_last_header;
    }
    else
    {
        last_header->size = heap_end - (u32int_t)last_header;
        printk("Expanded to %d Bytes\n", last_header->size);
        return last_header;
    }
}

void print_blocks()
{
    heap_header_t *cur = heap_head;
    while (cur)
    {
        printk("Address: 0x%08X\t Allocated: %d\t Size: %d B\n", cur, cur->allocated, cur->size);
        cur = cur->next;
    }
}