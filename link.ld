ENTRY(start)                /* the name of the entry label */

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */
    PROVIDE( kernel_start_offset = . );

    .init.text : 
    {
        *(.init.text)
        . = ALIGN(4096);
    }
    .init.data : 
    {
        *(.init.data)
        . = ALIGN(4096);
    }
    . += 0xC0000000;
    .text : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
        . = ALIGN(4096);
    }
    .rodata : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
        . = ALIGN(4096);
    }
    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
        . = ALIGN(4096);
    }
    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
        . = ALIGN(4096);
    }
    PROVIDE( kernel_end_offset = . );
  
}
