ENTRY(_start)                /* 入口label */

SECTIONS {
    . = 0x00100000;          /* loader会被grub加载到1MB的位置 */
    .init.text : 
    {
        *loader*(.init.text)
        *init*(.init.text)
    }
    .init.data : 
    {
        *(.init.data)
    }
    . += 0xC0000000;
    .text : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    }
    .rodata : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
    }
    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }
    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)

    }
    PROVIDE(kernel_image_end_vaddr = .);
  
}
