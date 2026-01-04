#pragma once
#include <stdint.h>
#include <io/io.h>
#include <stdbool.h>

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64
#define BUFFER_SIZE     64

// --- Initialize the keyboard ---
// Registers IRQ1 and prepares internal buffer.
void keyboard_init(void);

// --- Read a scancode from the internal buffer ---
// Returns 0 if buffer is empty.
uint8_t keyboard_read_scancode(void);

// --- Get a single ASCII character ---
// Converts scancode to ASCII using Shift and CapsLock states.
char keyboard_getchar(void);


