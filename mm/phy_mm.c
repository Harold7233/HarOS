#include <mm/phy_mm.h>
#include <common/dev.h>
frame_t phy_pages[PAGE_MAX_COUNT];
frame_t *free_list;
frame_t *alloc_frame()
{
    if (free_list)
    {   
        frame_t *head = free_list;
        free_list = free_list->next;
        // printk("Allocated frame at 0x%08X\n", head->addr);
        return head;
    }
    else
    {
        PANIC("Out of frames!");
    }
}
void free_frame(frame_t *frame)
{
    if (free_list)
    {
        frame->next = free_list;
    }
    free_list = frame;
}
void show_phy_mem(multiboot_info_t *mboot_ptr)
{
    u32int_t mmap_addr = mboot_ptr->mmap_addr;
    u32int_t mmap_length = mboot_ptr->mmap_length;
    memory_map_t *mmap = (memory_map_t *)mmap_addr;
    printk("Available physical memory segments: \n");
    for (mmap = mmap_addr; (u32int_t)mmap < mmap_addr + mmap_length; mmap++)
    {
        printk("Base address: 0x%08X  ", mmap->base_addr_low);
        if (mmap->length_low / 1024 / 1024 >= 1)
        {
            printk("Length: 0x%08X (%6dMB)  ", mmap->length_low, mmap->length_low / 1024 / 1024);
        }
        else
        {
            printk("Length: 0x%08X (%6dKB)  ", mmap->length_low, mmap->length_low / 1024);
        }

        printk("Type: %d\n", mmap->type);
    }
}

void init_frames(multiboot_info_t *mboot_ptr)
{
    int count = 0;
    u32int_t mmap_addr = mboot_ptr->mmap_addr;
    u32int_t mmap_length = mboot_ptr->mmap_length;
    memory_map_t *mmap = (memory_map_t *)mmap_addr;
    for (mmap = mmap_addr; (u32int_t)mmap < mmap_addr + mmap_length; mmap++)
    {
        if (mmap->type != 1)
            continue;
        u32int_t upper_bound = (mmap->base_addr_low + mmap->length_low) & PAGE_ALIGN_MASK;
        u32int_t base = MAX(mmap->base_addr_low, FREE_PHY_STARTS_AT);
        if (base & PAGE_ALIGN_MASK < base)
        {
            base = (base + 4096) & PAGE_ALIGN_MASK;
        }

        u32int_t t = base;
        
        while (t < upper_bound)
        {
            frame_t *n = &phy_pages[FRAME_TO_INDEX(t)];
            n->addr = t;
            n->next = NULL;
            free_frame(n);
            t += 4096;
            ++count;
        }
    }
   // printk("Total frames: %d\n", count);
}
