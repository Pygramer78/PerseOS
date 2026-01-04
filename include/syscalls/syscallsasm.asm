; syscall.asm
; Syscall handler for int 0x80 calls

[BITS 32]
global syscall_handler
extern c_syscall_handler  ; C function that handles syscalls

section .text

syscall_handler:
    pusha               ; saves eax, ebx, ecx, edx, esi, edi, ebp, esp
    push ds
    push es
    push fs
    push gs

    ; Calls the C function: eax=number, ebx/ecx/edx=args
    call c_syscall_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa

    iret