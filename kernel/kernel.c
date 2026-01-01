#include <vga/vga.h>

/*
kernel.c: The core of the OS. With it, the bootloader would be useless
*/

void kernel_main(void) {
    clear();
    const char* msg = "64-bit ELF Kernel iniciated correctly";
    print(msg);

    while (1) {
        __asm__("hlt");
    }
}