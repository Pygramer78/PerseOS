#include "keyboard.h"

// --- Circular buffer for scancodes ---
static uint8_t buffer[BUFFER_SIZE];
static uint8_t head = 0; // next write position
static uint8_t tail = 0; // next read position

// -- Debounce program --
static uint32_t last_keypress_time = 0;
static uint32_t key_repeat_delay = 500;
static uint32_t repeat_start_time = 0;
static bool is_repeating = false;

// --- Modifier flags ---
static bool shift = false;
static bool capslock = false;

// --- Push scancode into buffer (internal) ---
static void push(uint8_t sc) {
    uint8_t next = (head + 1) % BUFFER_SIZE;
    if (next != tail) {  // prevent overwrite if full
        buffer[head] = sc;
        head = next;
    }
}

// --- Pop scancode from buffer (internal) ---
static uint8_t pop(void) {
    if (head == tail) return 0;  // buffer empty
    uint8_t sc = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return sc;
}

// --- IRQ1 handler for keyboard (internal) ---
// Reads scancode from hardware and pushes it into the buffer.
// Should be registered with the IDT in keyboard_init().
void keyboard_irq_handler(void) {
    uint8_t sc = inb(KBD_DATA_PORT); // read scancode from keyboard port
    push(sc);                        // store in buffer
}

// --- Public functions ---

uint8_t keyboard_read_scancode(void) {
    return pop();
}

// --- Get ASCII character from keyboard ---
// Loops until a valid character is available.
char keyboard_getchar(void) {
    char c = 0;
    bool shift = false, capslock = false;

    while (!c) {
        uint8_t sc = keyboard_read_scancode();
        if (!sc) continue; // wait for scancode

        unsigned int current_time = get_current_time(); // Get current time

        // Debouncing: Check if enough time has passed since the last key press
        if (current_time - last_keypress_time < DEBOUNCE_DELAY) {
            continue;  // Skip this key press, not enough time has passed for debouncing
        }

        // Handle Shift and CapsLock
        if (sc == 0x2A || sc == 0x36) { shift = true; continue; }   // Shift pressed
        if (sc == 0xAA || sc == 0xB6) { shift = false; continue; }  // Shift released
        if (sc == 0x3A) { capslock = !capslock; continue; }         // CapsLock toggle

        // If key is released, reset any flags
        if (sc == 0xAA || sc == 0xB6 || sc == 0x3A) continue; // Ignore release codes

        // Convert scancode to ASCII
        c = scancode_to_ascii(sc, shift, capslock);

        // Handle key repeat logic
        if (current_time - repeat_start_time >= REPEAT_DELAY && !is_repeating) {
            // Start key repeat
            repeat_start_time = current_time;
            is_repeating = true;
        }

        if (is_repeating && current_time - repeat_start_time >= REPEAT_INTERVAL) {
            // Repeat the key at fixed intervals
            c = scancode_to_ascii(sc, shift, capslock);
            repeat_start_time = current_time;  // Reset repeat timer
        }

        // Update the last keypress time
        last_keypress_time = current_time;
    }

    return c;
}
