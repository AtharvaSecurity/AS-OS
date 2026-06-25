#ifndef VESA_H
#define VESA_H

#include "stdint.h"

#define VESA_WIDTH 1024
#define VESA_HEIGHT 768
#define VESA_BPP 32

typedef struct {
    uint32_t* framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
} vesa_mode_t;

// VESA functions
void vesa_init();
void vesa_put_pixel(int x, int y, uint32_t color);
void vesa_fill_rect(int x, int y, int w, int h, uint32_t color);
void vesa_draw_rect(int x, int y, int w, int h, uint32_t color);
void vesa_draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void vesa_draw_char(int x, int y, char c, uint32_t color);
void vesa_draw_string(int x, int y, const char* str, uint32_t color);
void vesa_clear_screen(uint32_t color);
void vesa_swap_buffers();

// Colors (ARGB format)
#define COLOR_BLACK       0xFF000000
#define COLOR_WHITE       0xFFFFFFFF
#define COLOR_RED         0xFFFF0000
#define COLOR_GREEN       0xFF00FF00
#define COLOR_BLUE        0xFF0000FF
#define COLOR_CYAN        0xFF00FFFF
#define COLOR_MAGENTA     0xFFFF00FF
#define COLOR_YELLOW      0xFFFFFF00
#define COLOR_GRAY        0xFF808080
#define COLOR_DARK_GRAY   0xFF404040
#define COLOR_LIGHT_GRAY  0xFFC0C0C0
#define COLOR_ORANGE      0xFFFF8000
#define COLOR_PURPLE      0xFF8000FF
#define COLOR_TEAL        0xFF008080

#endif
