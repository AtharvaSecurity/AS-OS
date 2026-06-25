#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "stdint.h"
#include "stddef.h"
#include "terminal.h"

extern "C" {
    void p(int x, int y, char c, uint8_t cl);
    void w(int x, int y, const char* s, uint8_t cl);
    void box(int x, int y, int w_, int h_, uint8_t bd, uint8_t bg);
}

#endif
