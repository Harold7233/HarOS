ENTRY(start)                /* the name of the entry label */

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */
    PROVIDE( kernel_start_offset = . );

    .text : 
    {
        *(.text)
        . = ALIGN(4096);
    }
    .rodata : 
    {
        *(.rodata)
        . = ALIGN(4096);
    }
    .data : 
    {
        *(.data)
        . = ALIGN(4096);
    }
    .bss : 
    {
        *(COMMON)
        *(.bss)
        . = ALIGN(4096);
    }
    PROVIDE( kernel_end_offset = . );
}
