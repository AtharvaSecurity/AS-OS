#ifndef CURSOR_H
#define CURSOR_H

#include "stdint.h"
#include "stdbool.h"

// Initialize cursor
void cursor_init();

// Show/hide cursor
void cursor_show();
void cursor_hide();

// Set cursor position
void cursor_set_pos(int x, int y);

// Get cursor position
void cursor_get_pos(int* x, int* y);

// Draw cursor at position
void cursor_draw(int x, int y);

// Update cursor (call in interrupt or main loop)
void cursor_update();

// Enable/disable cursor blinking
void cursor_set_blink(bool enable);
void cursor_set_blink_rate(int ms);

// Get cursor visibility
bool cursor_is_visible();

#endif // CURSOR_H
