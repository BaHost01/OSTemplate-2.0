#include "keyboard.h"
#include "io.h"

static const char scancode_table[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char keyboard_get_char() {
    while (!(inb(0x64) & 1)); // Wait for data
    uint8_t scancode = inb(0x60);
    if (scancode < sizeof(scancode_table)) {
        return scancode_table[scancode];
    }
    return 0;
}
