#ifndef PHY_MM_H
#define PHY_MM_H
#include <common/common.h>
#define PAGE_SIZE 4096
#define PHY_MAX_SIZE  0x80000000
#define PAGE_ALIGN_MASK 0xFFFFF000
#define PAGE_MAX_COUNT (PHY_MAX_SIZE/PAGE_SIZE)
#define FRAME_TO_INDEX(x) (x/4096)
#define FREE_PHY_STARTS_AT 0x11000000
#include "multiboot.h"
typedef struct frame{
    u32int_t addr;
    u32int_t *next;
}frame_t;


frame_t *alloc_frame();
void free_frame(frame_t *);
void init_frames(multiboot_info_t *);
void show_phy_mem(multiboot_info_t *);
#endif 