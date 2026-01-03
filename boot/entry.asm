BITS 32
global _start
extern kernel_main

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
_start:
    cli
    mov esp, stack_top
    call kernel_main

.hang:
    hlt
    jmp .hang
