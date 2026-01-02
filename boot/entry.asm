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
    lea rsp, [stack_top]
    call kernel_main

.hang:
    hlt
    jmp .hang
