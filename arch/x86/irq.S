.macro IRQ_TEMPLATE irq_num, isr_num
.global irq\irq_num
irq\irq_num:
    cli
    pushl $\irq_num
    pushl $\isr_num
    jmp enter_irq
.endm

enter_irq:
    pushal
    movw %ds, %dx
    pushw %dx
    movw $0x10, %dx
    movw %dx, %ds
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    call intr_handler
    popw %dx
    movw %dx, %ds
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    popal
    add $8, %esp
    sti
    iret

IRQ_TEMPLATE 0,32
IRQ_TEMPLATE 1,33
IRQ_TEMPLATE 2,34
IRQ_TEMPLATE 3,35
IRQ_TEMPLATE 4,36
IRQ_TEMPLATE 5,37
IRQ_TEMPLATE 6,38
IRQ_TEMPLATE 7,39
IRQ_TEMPLATE 8,40
IRQ_TEMPLATE 9,41
IRQ_TEMPLATE 10,42
IRQ_TEMPLATE 11,43
IRQ_TEMPLATE 12,44
IRQ_TEMPLATE 13,45
IRQ_TEMPLATE 14,45
IRQ_TEMPLATE 15,47
