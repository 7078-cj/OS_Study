.set MAGIC, 0x1BADB002
.set ALIGN,    1<<0             
.set MEMINFO,  1<<1            
.set FLAGS,    ALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern kernelMain # this tells assembler there is kernelMain function
.extern callConstructors

.global loader

loader:
    mov $kernel_stack, %esp
    call callConstructors
    push %eax
    push %ebx
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024 # 2Mib
kernel_stack: