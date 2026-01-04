; idt_flush.asm
[BITS 32]
global idt_flush

idt_flush:
    ; argument: [esp] = pointer to idt_ptr_t
    mov eax, [esp + 4]  ; load pointer
    lidt [eax]
    ret
