#include "gui.h"
#include "mouse.h"
#include "terminal.h"
#include "cursor.h"
#include "graphics.h"
#include "stddef.h"

// Color definitions
#define BG  vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
#define CY  vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK)
#define GR  vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK)
#define RD  vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK)
#define YL  vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK)
#define MG  vga_entry_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK)
#define BL  vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK)
#define WH  vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
#define GRY vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY)
#define PAN vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY)
#define TIT vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE)

// GUI window management
#define MAX_WINDOWS 10

static window_t windows[MAX_WINDOWS];
static int window_count = 0;

// Initialize GUI
void gui_init() {
    window_count = 0;
    for(int i = 0; i < MAX_WINDOWS; i++) {
        windows[i].visible = false;
    }
}

// Create a window
window_t* gui_create_window(int x, int y, int w, int h, const char* title) {
    if(window_count >= MAX_WINDOWS) return 0;
    
    window_t* win = &windows[window_count++];
    win->x = x;
    win->y = y;
    win->width = w;
    win->height = h;
    win->visible = true;
    win->active = true;
    win->border_color = CY;
    win->bg_color = BG;
    win->title_color = TIT;
    
    for(int i = 0; title[i] && i < 31; i++) {
        win->title[i] = title[i];
        win->title[i+1] = '\0';
    }
    
    return win;
}

// Draw a window
void gui_draw_window(window_t* win) {
    if(!win || !win->visible) return;
    
    // Draw window using the box function
    box(win->x, win->y, win->width, win->height, win->border_color, win->bg_color);
    
    // Title
    w(win->x + 2, win->y + 1, win->title, WH);
}

// Close window
void gui_close_window(window_t* win) {
    if(win) {
        win->visible = false;
    }
}

// Handle mouse click
void gui_handle_mouse_click(int x, int y, int button) {
    (void)button;
    
    for(int i = window_count - 1; i >= 0; i--) {
        window_t* win = &windows[i];
        if(!win->visible) continue;
        
        if(x >= win->x && x <= win->x + win->width &&
           y >= win->y && y <= win->y + win->height) {
            window_t temp = windows[i];
            for(int j = i; j < window_count - 1; j++) {
                windows[j] = windows[j+1];
            }
            windows[window_count - 1] = temp;
            
            for(int j = 0; j < window_count; j++) {
                windows[j].active = (j == window_count - 1);
            }
            break;
        }
    }
}

void gui_handle_mouse_move(int x, int y) {
    (void)x;
    (void)y;
}

void gui_update() {
    mouse_state_t* mouse = mouse_get_state();
    
    if(mouse->left_clicked) {
        gui_handle_mouse_click(mouse->x, mouse->y, 1);
    }
    
    if(mouse->moved) {
        gui_handle_mouse_move(mouse->x, mouse->y);
    }
}

void gui_render() {
    for(int i = 0; i < window_count; i++) {
        if(windows[i].visible) {
            gui_draw_window(&windows[i]);
        }
    }
    
    gui_draw_taskbar();
    mouse_draw();
}

void gui_draw_taskbar() {
    for(int x = 0; x < 80; x++) {
        p(x, 24, ' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY));
    }
    
    const char* start = "[Start]";
    for(int i = 0; start[i]; i++) {
        p(1 + i, 24, start[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
    }
    
    const char* time = "10:30 PM";
    int time_x = 80 - 10;
    for(int i = 0; time[i]; i++) {
        p(time_x + i, 24, time[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY));
    }
}

void gui_draw_desktop() {
    for(int y = 0; y < 24; y++) {
        vga_color shade = (y < 6) ? VGA_COLOR_BLUE : 
                          (y < 12) ? VGA_COLOR_MAGENTA : 
                          (y < 18) ? VGA_COLOR_CYAN : VGA_COLOR_GREEN;
        for(int x = 0; x < 80; x++) {
            p(x, y, ' ', vga_entry_color(VGA_COLOR_WHITE, shade));
        }
    }
}

void gui_draw_start_menu() {
    box(2, 15, 30, 9, CY, BG);
    w(4, 16, "Programs", WH);
    w(4, 17, "Documents", WH);
    w(4, 18, "Settings", WH);
    w(4, 19, "Run...", WH);
    w(4, 20, "Shutdown", WH);
}

void gui_alert(const char* message) {
    int len = strlen(message);
    int width = len + 6;
    if(width > 70) width = 70;
    int x = (80 - width) / 2;
    int y = 10;
    
    box(x, y, width, 5, RD, BG);
    w(x + 2, y + 2, message, WH);
    w(x + (width - 10) / 2, y + 3, "[ OK ]", GR);
    delay(1000);
}

void gui_progress_bar(int x, int y, int width, int percent, uint8_t color) {
    for(int i = 0; i < width; i++) {
        p(x + i, y, ' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }
    
    int filled = (percent * width) / 100;
    for(int i = 0; i < filled; i++) {
        p(x + i, y, '=', color);
    }
    
    p(x, y, '[', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    p(x + width - 1, y, ']', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
}
