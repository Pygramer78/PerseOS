#pragma once
/*
Welcome to syscalls!!!
This is probably the most exciting thing of this kernel.
If you're asking yourself what number is the interruption of the syscall is the same as linux
int 0x80.
Remember to use 32-bit assembly!
*/
#include <vga/vga.h>
/* All syscalls on PerseOS and their numbers */
#define SYS_EXIT 1
#define SYS_WRITE 2
#define SYS_READ 3
#define SYS_CLEAR 4
/* ----------------------------------------- */
uint32_t c_syscall_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx); // In syscalls.c you can find all syscalls functions