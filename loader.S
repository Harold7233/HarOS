.equ MBOOT_PAGE_ALIGN, 1
.equ MBOOT_MEM_INFO, 2
.equ MBOOT_HEADER_MAGIC, 0x1BADB002 // GRUB magic

.equ MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
.equ MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

.global mboot
//.extern tss, bss, end //gas会忽略，但是标注一下

.section .init.text

mboot: // 在代码段的起始位置设置符合 Multiboot 规范的标记
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_CHECKSUM

.global _start // loader入口
.global mboot_ptr
.extern init_paging // C语言函数
_start:
    cli
    movl %ebx, (mboot_ptr)
    movl STACK_TOP, %esp
    andl 0xFFFFFFF0, %esp //esp按照16字节对齐
    movl 0, %ebp
    cli
    call init_paging
hlt:
    jmp hlt

.section .init.data
    stack:
    .lcomm temp_stack, 1024
    stack_end:
    .equ STACK_TOP, stack_end-stack-1
    mboot_ptr:
    .long 0
