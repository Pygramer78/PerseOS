#include <vga/vga.h>
#include <idt/idt.h>
#include <syscalls/syscalls.h>
#define BUFFER_LENGTH 128
/*
kernel.c: The core of the OS. With it, the bootloader would be useless
*/

void kernel_main(void) {
    idt_init();

    clear();
    const char* msg = "32-bit ELF Kernel iniciated correctly, Hello World! \n";
    print(msg);
    print("Welcome to PerseOS! Write something \n");
    char buffer[BUFFER_LENGTH];
    input(buffer, BUFFER_LENGTH);
    while (1) {
        __asm__ volatile ("hlt"); // Stop untils the computer shuts off
    }
}