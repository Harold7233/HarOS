#ifndef PAGING_H
#define PAGING_H
#include <common/common.h>
#include <mm/phy_mm.h>
typedef u32int_t page_t;
// {
//     u32int_t present : 1;  // Page present in memory
//     u32int_t rw : 1;       // Read-only if clear, readwrite if set
//     u32int_t user : 1;     // Supervisor level only if clear
//     u32int_t accessed : 1; // Has the page been accessed since last refresh?
//     u32int_t dirty : 1;    // Has the page been written to since last refresh?
//     u32int_t unused : 7;   // Amalgamation of unused and reserved bits
//     u32int_t frame : 20;   // Frame address (shifted right 12 bits)
// } ;

/*页表，10位所以有1024项*/
typedef struct page_table
{
    u32int_t pages[1024];
} page_table_t;
/*页目录，10位所以有1024项*/
typedef struct page_directory
{
    page_table_t *tables[1024];
    page_table_t *tables_linear[1024];
    // u32int_t physicalAddr;
} page_directory_t;

#define KERNEL_START_LINEAR 0xC0100000
#define KERNEL_START_PHYSICAL 0xC0100000
#define KERNEL_OFFSET 0xC0000000
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4
#define PAGE_KERNEL 0
#define PAGE_ACCESSED 0x8
#define PAGE_DIRTY 0x10
#define PDE_INDEX(x) ((u32int_t)x >> 22)
#define PTE_INDEX(x) (((u32int_t)x >> 12) & 0x3FF)
#define KERNEL_STACK_SIZE 256 * PAGE_SIZE

#define KERNEL_PAGE_TABLE_COUNT 64
// page_t *get_page(u32int_t linear_addr, page_directory_t *dir);

void init_paging();
void switch_page_directory(u32int_t);
u32int_t get_page(page_directory_t *, u32int_t); 
void map(page_directory_t *, u32int_t, frame_t *, u16int_t);
void alloc_map(page_directory_t *, u32int_t, u16int_t);
extern page_directory_t kernel_page_dir;
#endif