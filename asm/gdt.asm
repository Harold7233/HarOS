[GLOBAL load_gdt]    ; Allows the C code to call gdt_flush().
[bits 32]
load_gdt:
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, (2<<3)      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
    
   mov ss, ax
   jmp (1<<3):dummy_ret   ; 0x08 is the offset to our code segment: Far jump!
dummy_ret:
   ret