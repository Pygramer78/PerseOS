global _start
extern kernel_main

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
_start:
    mov rsp, stack_top
    mov rbp, 0

    call kernel_main

.hang:
    hlt
    jmp .hang
