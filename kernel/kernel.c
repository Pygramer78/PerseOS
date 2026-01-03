#include <vga/vga.h>

/*
kernel.c: The core of the OS. With it, the bootloader would be useless
*/

void kernel_main(void) {
    __asm__ volatile ("cli");

    clear();
    const char* msg = "32-bit ELF Kernel iniciated correctly, Hello World!";
    print(msg);

    while (1) {
        __asm__ volatile ("hlt");
    }
}