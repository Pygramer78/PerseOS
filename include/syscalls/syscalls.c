#include "syscalls.h"

uint32_t c_syscall_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
    switch (eax) {
        case SYS_WRITE:
            print((const char*)ebx);
            return 0;

        case SYS_READ:
            input((char*)ebx, ecx);
            return 0;
        
        case SYS_CLEAR:
            clear();
            return 0;
        
        case SYS_EXIT:
            while (1) { __asm__ volatile ("hlt"); }
            return 0;
        default:
            return -1; // Unknown syscall
    }
}