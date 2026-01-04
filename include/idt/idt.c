#include "idt.h"

#define IDT_SIZE 256

idt_entry_t idt[IDT_SIZE];
idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = sel;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Inicializa todas las entradas a 0
    for(int i = 0; i < IDT_SIZE; i++)
        idt_set_gate(i, 0, 0, 0);

    // Ejemplo: vector 0x80 → syscall
    extern void syscall_handler();  // desde syscall.asm
    idt_set_gate(0x80, (uint32_t)syscall_handler, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr); // carga la IDT en el CPU
}
