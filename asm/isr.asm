%macro ISR_TEMPLATE 2 ; 第一个参数是中断号，第二个参数是CPU是否会push 错误号
  [GLOBAL isr%1]
  isr%1:
    cli               ; 关中断
    %if %2 == 0
    push byte 0
    %endif
    push %1

    jmp isr_wrapper
%endmacro
[extern isr_handler_c]
isr_wrapper:
    pusha             ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov dx, ds        ; Lower 16-bits of eax = ds.
    push edx          ; save the data segment descriptor
    mov dx, 0x10      ; load the kernel data segment descriptor
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx

    call isr_handler_c

    pop edx           ; reload the original data segment descriptor
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx

    popa              ; Pops edi,esi,ebp...
    add esp, 8        ; Cleans up the pushed error code and pushed ISR number
    sti

    iret              ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


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
ISR_TEMPLATE 13,0
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