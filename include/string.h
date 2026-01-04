#pragma once
#include <stddef.h>
#include <stdint.h>
/*
Why did i make this string.h instead of the normal libc?
This kernel uses the compiler in a way so that libc isn't automatically included
*/

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
int   memcmp(const void* a, const void* b, size_t n);
