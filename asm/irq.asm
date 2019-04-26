%macro IRQ_TEMPLATE 2 ; 第一个参数是IRQ号，第二个参数是对应的ISR号
  [GLOBAL irq%1]
  irq%1:
    cli               ; 关中断
    push byte %1
    push byte %2
    jmp irq_wrapper
%endmacro
[extern irq_handler_c]
irq_wrapper:
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   
   call irq_handler_c

   pop ebx        ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
 




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
