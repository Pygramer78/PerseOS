#include "vga.h"
/*
vga.c: Where the vga functions code resides
*/
static size_t row = 0;
static size_t col = 0;

static vga_color fgcolor = VGA_WHITE;
static vga_color bgcolor = VGA_BLACK;

void clear(void) {
    uint8_t color = vga_entry_color(fgcolor, bgcolor);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA[y * VGA_WIDTH + x] =
                vga_entry(' ', color);
        }
    }

    row = 0;
    col = 0;
}

void print(const char* str) {
    uint8_t color = vga_entry_color(fgcolor, bgcolor);

    for (size_t i = 0; str[i]; i++) {
        VGA[row * VGA_WIDTH + col] =
            vga_entry(str[i], color);

        col++;
        if (col >= VGA_WIDTH) {
            col = 0;
            row++;
        }
    }
}
