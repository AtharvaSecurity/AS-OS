#include "keyboard.h"
#include "terminal.h"

extern "C" {
    uint8_t inb(uint16_t port);
}

#define KEYBOARD_BUFFER_SIZE 256
static char key_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;
static bool shift_pressed = false;
static bool caps_lock = false;

static const char scancode_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static const char scancode_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

void keyboard_initialize() {
    buffer_head = 0;
    buffer_tail = 0;
    shift_pressed = false;
    caps_lock = false;
}

bool keyboard_has_char() {
    return buffer_head != buffer_tail;
}

char keyboard_read_char() {
    while (buffer_head == buffer_tail) {
        keyboard_update();
    }
    char c = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_update() {
    if (inb(0x64) & 0x01) {
        uint8_t scancode = inb(0x60);
        
        if (scancode & 0x80) {
            uint8_t key = scancode & 0x7F;
            if (key == 0x2A || key == 0x36) shift_pressed = false;
            return;
        }
        
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
            return;
        }
        
        if (scancode == 0x3A) {
            caps_lock = !caps_lock;
            return;
        }
        
        if (scancode < sizeof(scancode_ascii)) {
            char c;
            bool shifted = shift_pressed ^ caps_lock;
            c = shifted ? scancode_shift[scancode] : scancode_ascii[scancode];
            
            if (c != 0) {
                int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
                if (next != buffer_tail) {
                    key_buffer[buffer_head] = c;
                    buffer_head = next;
                }
            }
        }
    }
}

void keyboard_read_line(char* buffer, int max_len) {
    int pos = 0;
    buffer[0] = '\0';
    
    while (1) {
        keyboard_update();
        if (keyboard_has_char()) {
            char c = keyboard_read_char();
            
            if (c == '\n') {
                buffer[pos] = '\0';
                terminal_putchar('\n');
                return;
            }
            else if (c == '\b') {
                if (pos > 0) {
                    pos--;
                    terminal_putchar('\b');
                }
            }
            else if (c >= ' ' && pos < max_len - 1) {
                buffer[pos++] = c;
                terminal_putchar(c);
                buffer[pos] = '\0';
            }
        }
    }
}
