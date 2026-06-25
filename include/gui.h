#ifndef GUI_H
#define GUI_H

#include "stdint.h"
#include "stdbool.h"
#include "terminal.h"

// Window structure
typedef struct window_t {
    int x, y;
    int width, height;
    char title[32];
    bool visible;
    bool active;
    uint8_t border_color;
    uint8_t bg_color;
    uint8_t title_color;
} window_t;

// GUI functions
#ifdef __cplusplus
extern "C" {
#endif

void gui_init();
window_t* gui_create_window(int x, int y, int w, int h, const char* title);
void gui_draw_window(window_t* win);
void gui_close_window(window_t* win);
void gui_handle_mouse_click(int x, int y, int button);
void gui_handle_mouse_move(int x, int y);
void gui_update();
void gui_render();
void gui_draw_taskbar();
void gui_draw_desktop();
void gui_draw_start_menu();
void gui_alert(const char* message);
void gui_progress_bar(int x, int y, int width, int percent, uint8_t color);

#ifdef __cplusplus
}
#endif

#endif // GUI_H
