#include <boot/multiboot.h>
#include <sys/asm.hpp>
#include <kernel/kernel.hpp>
#include <arch/x86/paging.hpp>
#include <sys/gcc.hpp>

#define INIT_MAP_SIZE MB(16)
page_directory_t INIT_DATA PAGE_ALIGNED init_pg_dir;
page_table_t INIT_DATA PAGE_ALIGNED init_pg_tables[6];
namespace kernel {
    void kmain(multiboot_info_t *);

    page_directory_t *kernel_pg_dir = reinterpret_cast<page_directory_t *>(PA(&init_pg_dir));
}
extern "C" {
uint8_t KERNEL_TEMP_STACK[8096];
extern multiboot_info_t *mboot_ptr;
void INIT_TEXT UNUSED init_paging() {

    /*临时映射0-4M到物理地址0-4M，因为开启分页后EIP的地址还是物理地址*/
    init_pg_dir.tables[0] = U32(&init_pg_tables[0]) | PAGE_PRESENT | PAGE_USER;
    uint32_t i = 0;
    while (i < 1024) {
        init_pg_tables[0].pages[i] = (i << 12) | PAGE_PRESENT | PAGE_RW | PAGE_USER;  // 第i个页的虚拟地址，并设置present和rw
        ++i;
    }

    /*临时映射3G~3G+16M至物理地址0-16M*/
    for (size_t table_i = 0; table_i < INIT_MAP_SIZE / MB(4); ++table_i) {
        page_table_t *table = &init_pg_tables[table_i + 1];
        for (int j = 0; j < 1024; ++j) {
            table->pages[j] = ((j + table_i * 1024) * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
        }
        init_pg_dir.tables[PAGE_TABLE_INDEX(KERNEL_VADDR_OFFSET + (table_i << 22))] =
                U32(table) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    }

    asm volatile("movl %0, %%cr3\n"
                 "movl %%cr0, %%eax\n"
                 "orl $0x80000000, %%eax\n" //开启分页
                 "movl %%eax, %%cr0\n"
                 "movl %1, %%esp\n"
                 ""::"r"(&init_pg_dir), "r"(KERNEL_TEMP_STACK + sizeof(KERNEL_TEMP_STACK)));
    kmain(mboot_ptr);
}
}