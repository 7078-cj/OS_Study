.set IRQ_BASE, 0x20

.section .text

.extern InterruptManager_HandleInterrupt


# --------------------------------------------------
# Ignore interrupt
# --------------------------------------------------

.global IgnoreInterruptRequest

IgnoreInterruptRequest:
    iret


# --------------------------------------------------
# CPU exception handler macro
# --------------------------------------------------

.macro HandleException num
.global HandleException\num\()

HandleException\num\():
    movb $\num, interruptNumber
    jmp int_bottom
.endm


# --------------------------------------------------
# Hardware IRQ handler macro
# --------------------------------------------------

.macro HandleInterruptRequest num
.global HandleInterruptRequest\num\()

HandleInterruptRequest\num\():
    movb $(\num + IRQ_BASE), interruptNumber
    jmp int_bottom
.endm


# --------------------------------------------------
# Generate IRQ handlers
# --------------------------------------------------

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01


# --------------------------------------------------
# Common interrupt handler
# --------------------------------------------------

int_bottom:
    pusha

    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    pushl interruptNumber

    call InterruptManager_HandleInterrupt

    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds

    popa

    iret


# --------------------------------------------------
# Data
# --------------------------------------------------

.section .data

interruptNumber:
    .byte 0