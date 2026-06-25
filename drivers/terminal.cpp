#include "terminal.h"

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static bool cursor_visible = true;
static uint8_t cursor_color = 0;
// static uint32_t cursor_blink_counter = 0;

extern "C" {
    void outb(uint8_t value, uint16_t port);
    uint8_t inb(uint16_t port);
}

void update_hardware_cursor() {
    if (!cursor_visible) return;
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    outb(0x0F, 0x3D4);
    outb((uint8_t)(pos & 0xFF), 0x3D5);
    outb(0x0E, 0x3D4);
    outb((uint8_t)((pos >> 8) & 0xFF), 0x3D5);
}

void draw_software_cursor() {
    if (!cursor_visible) return;
    // Draw a visible cursor block at current position
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    char c = VGA_MEMORY[pos] & 0xFF;
    if (c == ' ' || c == 0) c = '_';
    VGA_MEMORY[pos] = vga_entry(c, cursor_color);
}

void clear_software_cursor() {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    VGA_MEMORY[pos] = vga_entry(' ', terminal_color);
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    cursor_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    cursor_visible = true;
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
    update_hardware_cursor();
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_set_cursor_color(uint8_t color) {
    cursor_color = color;
}

void terminal_putchar(char c) {
    // Remove old cursor
    if (cursor_visible) {
        VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);
    }
    
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
    else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);
        }
    }
    else if (c == '\r') {
        terminal_column = 0;
    }
    else {
        VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
        if (++terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row >= VGA_HEIGHT) {
                terminal_scroll();
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
    
    // Draw cursor at new position
    if (cursor_visible) {
        draw_software_cursor();
    }
    update_hardware_cursor();
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y-1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        VGA_MEMORY[(VGA_HEIGHT-1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }
}

void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    if (cursor_visible) draw_software_cursor();
    update_hardware_cursor();
}

void terminal_hide_cursor(void) {
    if (cursor_visible) {
        clear_software_cursor();
    }
    cursor_visible = false;
    outb(0x0F, 0x3D4);
    outb(0xFF, 0x3D5);
    outb(0x0E, 0x3D4);
    outb(0xFF, 0x3D5);
}

void terminal_show_cursor(void) {
    cursor_visible = true;
    cursor_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    draw_software_cursor();
    update_hardware_cursor();
}

void terminal_update_cursor(int x, int y) {
    if (cursor_visible) clear_software_cursor();
    if (x >= 0 && x < VGA_WIDTH) terminal_column = x;
    if (y >= 0 && y < VGA_HEIGHT) terminal_row = y;
    if (cursor_visible) draw_software_cursor();
    update_hardware_cursor();
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return 1;
        if (s1[i] == '\0') return 0;
    }
    return 0;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

void* memset(void* s, int c, size_t n) {
    unsigned char* p = (unsigned char*)s;
    for (size_t i = 0; i < n; i++) p[i] = (unsigned char)c;
    return s;
}

void delay(int count) {
    for (volatile int i = 0; i < count * 1000000; i++) {
        __asm__ volatile("nop");
    }
}
