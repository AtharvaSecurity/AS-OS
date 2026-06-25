#include "graphics.h"

#define VGA ((uint16_t*)0xB8000)

void p(int x, int y, char c, uint8_t cl) {
    if(x >= 0 && x < 80 && y >= 0 && y < 25)
        VGA[y*80+x] = vga_entry(c, cl);
}

void w(int x, int y, const char* s, uint8_t cl) {
    for(int i = 0; s[i]; i++)
        p(x+i, y, s[i], cl);
}

void box(int x, int y, int w_, int h_, uint8_t bd, uint8_t bg) {
    // Draw filled background
    for(int r = y+1; r < y+h_-1 && r < 25; r++) {
        for(int c = x+1; c < x+w_-1 && c < 80; c++) {
            p(c, r, ' ', bg);
        }
    }
    
    // Draw top/bottom borders
    for(int i = 0; i < w_ && x+i < 80; i++) {
        p(x+i, y, '-', bd);
        p(x+i, y+h_-1, '-', bd);
    }
    
    // Draw left/right borders
    for(int i = 1; i < h_-1; i++) {
        p(x, y+i, '|', bd);
        p(x+w_-1, y+i, '|', bd);
    }
    
    // Draw corners
    p(x, y, '+', bd);
    p(x+w_-1, y, '+', bd);
    p(x, y+h_-1, '+', bd);
    p(x+w_-1, y+h_-1, '+', bd);
}
