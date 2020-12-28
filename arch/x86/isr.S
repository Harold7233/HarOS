.macro ISR_TEMPLATE intr_num, push_err_code
.global isr\intr_num
isr\intr_num:
    cli
    .if \push_err_code == 0
    pushl $0
    .endif
    pushl $\intr_num
    jmp enter_isr
.endm

enter_isr:
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


ISR_TEMPLATE 0,0
ISR_TEMPLATE 1,0
ISR_TEMPLATE 2,0
ISR_TEMPLATE 3,0
ISR_TEMPLATE 4,0
ISR_TEMPLATE 5,0
ISR_TEMPLATE 6,0
ISR_TEMPLATE 7,0
ISR_TEMPLATE 8,0
ISR_TEMPLATE 9,0
ISR_TEMPLATE 10,0
ISR_TEMPLATE 11,0
ISR_TEMPLATE 12,0
ISR_TEMPLATE 13,1
ISR_TEMPLATE 14,1
ISR_TEMPLATE 15,0
ISR_TEMPLATE 16,0
ISR_TEMPLATE 17,0
ISR_TEMPLATE 18,0
ISR_TEMPLATE 19,0
ISR_TEMPLATE 20,0
ISR_TEMPLATE 21,0
ISR_TEMPLATE 22,0
ISR_TEMPLATE 23,0
ISR_TEMPLATE 24,0
ISR_TEMPLATE 25,0
ISR_TEMPLATE 26,0
ISR_TEMPLATE 27,0
ISR_TEMPLATE 28,0
ISR_TEMPLATE 29,0
ISR_TEMPLATE 30,0
ISR_TEMPLATE 31,0
ISR_TEMPLATE 32,0

ISR_TEMPLATE 128, 0
