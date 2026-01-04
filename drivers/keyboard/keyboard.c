#include "keyboard.h"

// --- Circular buffer for scancodes ---
static uint8_t buffer[BUFFER_SIZE];
static uint8_t head = 0; // next write position
static uint8_t tail = 0; // next read position

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
static void keyboard_irq_handler(void) {
    uint8_t sc = inb(KBD_DATA_PORT); // read scancode from keyboard port
    push(sc);                        // store in buffer
    outb(0x20, 0x20);                // send End Of Interrupt (EOI) to PIC
}

// --- Public functions ---

void keyboard_init(void) {
    // Register IRQ1 handler in your kernel's IDT
    // Example:
    // register_interrupt_handler(IRQ1, keyboard_irq_handler);
    // Ensure IRQ1 is enabled in the PIC.
}

uint8_t keyboard_read_scancode(void) {
    return pop();
}

// --- Get ASCII character from keyboard ---
// Loops until a valid character is available.
char keyboard_getchar(void) {
    char c = 0;

    while (!c) {
        uint8_t sc = keyboard_read_scancode();
        if (!sc) continue; // wait for scancode

        // --- Handle Shift and CapsLock ---
        if (sc == 0x2A || sc == 0x36) { shift = true; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift = false; continue; }
        if (sc == 0x3A) { capslock = !capslock; continue; }

        // Convert scancode to ASCII
        c = scancode_to_ascii(sc, shift, capslock); // you implement this
    }

    return c;
}
