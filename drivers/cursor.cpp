#include "cursor.h"
#include "terminal.h"

extern "C" {
    void outb(uint8_t value, uint16_t port);
    uint8_t inb(uint16_t port);
}

static bool cursor_visible = true;
static int cursor_x = 0;
static int cursor_y = 0;
static bool cursor_initialized = false;

// VGA cursor ports
#define CURSOR_PORT_CMD 0x3D4
#define CURSOR_PORT_DATA 0x3D5

void cursor_init() {
    cursor_initialized = true;
    cursor_show();
    cursor_set_pos(0, 0);
}

void cursor_show() {
    cursor_visible = true;
    // Enable cursor
    outb(CURSOR_PORT_CMD, 0x0A);
    outb(CURSOR_PORT_DATA, (inb(CURSOR_PORT_DATA) & 0xC0) | 0x00);
}

void cursor_hide() {
    cursor_visible = false;
    // Disable cursor
    outb(CURSOR_PORT_CMD, 0x0A);
    outb(CURSOR_PORT_DATA, 0x20);
}

void cursor_set_pos(int x, int y) {
    if(x < 0) x = 0;
    if(x >= 80) x = 79;
    if(y < 0) y = 0;
    if(y >= 25) y = 24;
    
    cursor_x = x;
    cursor_y = y;
    
    if(!cursor_initialized) return;
    
    uint16_t pos = y * 80 + x;
    outb(CURSOR_PORT_CMD, 0x0F);
    outb(CURSOR_PORT_DATA, (uint8_t)(pos & 0xFF));
    outb(CURSOR_PORT_CMD, 0x0E);
    outb(CURSOR_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void cursor_get_pos(int* x, int* y) {
    if(x) *x = cursor_x;
    if(y) *y = cursor_y;
}

void cursor_draw(int x, int y) {
    cursor_set_pos(x, y);
}

void cursor_update() {
    // Simple update - just maintain visibility
}

void cursor_set_blink(bool enable) {
    // Blink not implemented in this simple version
}

void cursor_set_blink_rate(int ms) {
    // Blink not implemented in this simple version
}

bool cursor_is_visible() {
    return cursor_visible;
}
