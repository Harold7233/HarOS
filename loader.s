MBOOT_PAGE_ALIGN    equ 1<<0       ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1       ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
                                   ; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
                                   ; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                          ; 所有代码以 32-bit 的方式编译
[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.
section .init.text                 ; 临时代码段从这里开始

                                   ; 在代码段的起始位置设置符合 Multiboot 规范的标记
mboot:
dd MBOOT_HEADER_MAGIC              ; GRUB 会通过这个魔数判断该映像是否支持
dd MBOOT_HEADER_FLAGS              ; GRUB 的一些加载时选项，其详细注释在定义处
dd MBOOT_CHECKSUM                  ; 检测数值，其含义在定义处

[GLOBAL start]                     ; 内核代码入口，此处提供该声明给 ld 链接器
[GLOBAL mboot_ptr]             ; 全局的 struct multiboot * 变量
[EXTERN init_paging_temp]               ; 声明内核 C 代码的入口函数
; [EXTERN kernel_main]
start:
    cli                            ; 此时还没有设置好保护模式的中断处理
                                      ; 所以必须关闭中断
    mov [mboot_ptr], ebx       ; 将 ebx 中存储的指针存入 glb_mboot_ptr 变量
    mov esp, STACK_TOP             ; 设置内核栈地址，按照 multiboot 规范
    and esp, 0FFFFFFF0H            ; 栈地址按照 16 字节对齐
    mov ebp, 0                     ; 帧指针修改为 0

    call init_paging_temp               ; 调用内核入口函数
    jmp $
section .init.data

stack:    times 1024 db 0          ; 这里作为临时内核栈
STACK_TOP equ $-stack-1            ; 内核栈顶，$ 符指代是当前地址

mboot_ptr: dd 0                ; 全局的 multiboot 结构体指针
