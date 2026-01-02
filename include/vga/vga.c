#include "vga.h"
/*
vga.c; Where the code of the header resides.
*/

void print(const char* text) {
    for (int i = 0; text[i]; i++) {
        VGA[i] = (0x0F << 8) | text[i];
    }
}

void clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        VGA[i] = (0x0F << 8) | ' ';
    }
}