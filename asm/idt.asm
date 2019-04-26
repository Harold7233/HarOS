[GLOBAL load_idt]    ; Allows the C code to call idt_flush().

load_idt:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
   lidt [eax]        ; Load the IDT pointer.
   ret