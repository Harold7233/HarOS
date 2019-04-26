#include <mm/paging.h>
#include <x86/interrupt.h>
#include <common/vga.h>
#include <libc/string.h>
#include <common/dev.h>
page_directory_t kernel_page_dir __attribute__((aligned(4096)));      // 4KB
page_table_t kernel_page_tables[1024] __attribute__((aligned(4096))); // 4MB

void init_paging()
{
    for (int i = 0; i < 1024; ++i)
    {
        page_table_t *table = &kernel_page_tables[i];
        kernel_page_dir.tables_linear[i] = table;
        kernel_page_dir.tables[i] = (page_table_t *)((((u32int_t)table - KERNEL_OFFSET) & PAGE_ALIGN_MASK) | PAGE_PRESENT | PAGE_RW | PAGE_USER);
    }
    memset(&kernel_page_tables, 0, sizeof(kernel_page_tables));

    u32int_t addr = KERNEL_OFFSET;
    /*将0xC0000000-0xFFFFFFFF（共1G）映射至0-0x3FFFFFFF*/

    for (int i = 0; i < KERNEL_PAGE_TABLE_COUNT; ++i)
    {
        page_table_t *t = &kernel_page_tables[PDE_INDEX(addr)];

        for (int j = 0; j < 1024; ++j)
        {
            t->pages[PTE_INDEX(addr)] = ((addr - KERNEL_OFFSET) & PAGE_ALIGN_MASK) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
            addr += PAGE_SIZE;
        }
    }
    switch_page_directory(((u32int_t)&kernel_page_dir) - KERNEL_OFFSET);
}
void switch_page_directory(u32int_t page_dir_ptr)
{
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(page_dir_ptr));
    u32int_t cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0"
                 :
                 : "r"(cr0));
}

void map(page_directory_t *dir, u32int_t v_addr, frame_t *frame, u16int_t attr)
{
    v_addr &= PAGE_ALIGN_MASK;

    // TODO table本身可能也需要映射？
    page_table_t *table = dir->tables_linear[PDE_INDEX(v_addr)];
    table->pages[PTE_INDEX(v_addr)] = frame->addr | attr;
    asm volatile("invlpg (%0)"
                 :
                 : "a"(v_addr));
}
u32int_t get_page(page_directory_t *dir, u32int_t v_addr)
{
    v_addr &= PAGE_ALIGN_MASK;

    u32int_t page = dir->tables_linear[PDE_INDEX(v_addr)]->pages[PTE_INDEX(v_addr)];
    if (page & PAGE_PRESENT)
    {
        return page;
    }
    return NULL;
}
void alloc_map(page_directory_t *dir, u32int_t v_addr, u16int_t attr)
{
    if (!get_page(dir, v_addr))
    {
        frame_t *new_frame = alloc_frame();
        map(dir, v_addr, new_frame, attr);
    }
}