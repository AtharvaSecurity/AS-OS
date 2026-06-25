#include "mouse.h"
#include "terminal.h"
#include "isr.h"

extern "C" {
    void outb(uint8_t value, uint16_t port);
    uint8_t inb(uint16_t port);
}

#define VGA ((uint16_t*)0xB8000)
#define MOUSE_PORT_DATA 0x60
#define MOUSE_PORT_STATUS 0x64
#define MOUSE_PORT_COMMAND 0x64

static mouse_state_t mouse_state;
static bool mouse_visible = true;

static void mouse_wait() {
    for(int i=0; i<100000; i++) {
        if((inb(MOUSE_PORT_STATUS) & 0x02) == 0) return;
    }
}

static void mouse_write(uint8_t data) {
    mouse_wait();
    outb(MOUSE_PORT_COMMAND, 0xD4);
    mouse_wait();
    outb(MOUSE_PORT_DATA, data);
}

static uint8_t mouse_read() {
    mouse_wait();
    return inb(MOUSE_PORT_DATA);
}

static void mouse_handler(registers_t* regs) {
    (void)regs; // Suppress unused parameter warning
    static uint8_t cycle = 0;
    static uint8_t data[3];
    uint8_t d = inb(MOUSE_PORT_DATA);
    
    switch(cycle) {
        case 0:
            data[0] = d;
            cycle++;
            break;
        case 1:
            data[1] = d;
            cycle++;
            break;
        case 2:
            data[2] = d;
            mouse_state.left_button = (data[0] & 0x01) != 0;
            mouse_state.right_button = (data[0] & 0x02) != 0;
            mouse_state.middle_button = (data[0] & 0x04) != 0;
            
            int dx = (int8_t)data[1];
            int dy = -(int8_t)data[2];
            
            mouse_state.last_x = mouse_state.x;
            mouse_state.last_y = mouse_state.y;
            
            mouse_state.x += dx;
            mouse_state.y += dy;
            
            if(mouse_state.x < 0) mouse_state.x = 0;
            if(mouse_state.x >= 80) mouse_state.x = 79;
            if(mouse_state.y < 0) mouse_state.y = 0;
            if(mouse_state.y >= 25) mouse_state.y = 24;
            
            mouse_state.moved = (dx != 0 || dy != 0);
            
            static bool last_left = false;
            static bool last_right = false;
            
            mouse_state.left_clicked = (mouse_state.left_button && !last_left);
            mouse_state.right_clicked = (mouse_state.right_button && !last_right);
            mouse_state.left_released = (!mouse_state.left_button && last_left);
            
            last_left = mouse_state.left_button;
            last_right = mouse_state.right_button;
            
            cycle = 0;
            break;
    }
}

void mouse_init() {
    mouse_state.x = 40;
    mouse_state.y = 12;
    mouse_state.last_x = 40;
    mouse_state.last_y = 12;
    mouse_state.left_button = false;
    mouse_state.right_button = false;
    mouse_state.middle_button = false;
    mouse_state.left_clicked = false;
    mouse_state.right_clicked = false;
    mouse_state.moved = false;
    mouse_state.wheel_delta = 0;
    
    mouse_wait();
    outb(MOUSE_PORT_COMMAND, 0xA8);
    mouse_wait();
    outb(MOUSE_PORT_COMMAND, 0x20);
    mouse_wait();
    uint8_t config = inb(MOUSE_PORT_DATA);
    config |= 0x02;
    mouse_wait();
    outb(MOUSE_PORT_COMMAND, 0x60);
    mouse_wait();
    outb(MOUSE_PORT_DATA, config);
    
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
    
    irq_install_handler(12, mouse_handler);
}

mouse_state_t* mouse_get_state() {
    return &mouse_state;
}

void mouse_set_position(int x, int y) {
    mouse_state.x = x;
    mouse_state.y = y;
    if(mouse_state.x < 0) mouse_state.x = 0;
    if(mouse_state.x >= 80) mouse_state.x = 79;
    if(mouse_state.y < 0) mouse_state.y = 0;
    if(mouse_state.y >= 25) mouse_state.y = 24;
}

void mouse_draw() {
    if(!mouse_visible) return;
    int x = mouse_state.x;
    int y = mouse_state.y;
    
    if(x >= 0 && x < 80 && y >= 0 && y < 25) {
        static uint16_t saved = 0;
        static int sx = -1, sy = -1;
        
        if(sx >= 0 && sy >= 0 && sx < 80 && sy < 25) {
            VGA[sy*80 + sx] = saved;
        }
        
        sx = x;
        sy = y;
        saved = VGA[y*80 + x];
        VGA[y*80 + x] = vga_entry(0xDB, vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_GREY));
    }
}

void mouse_show() { mouse_visible = true; }
void mouse_hide() { mouse_visible = false; }

void mouse_update() {
    mouse_state.left_clicked = false;
    mouse_state.right_clicked = false;
    mouse_state.moved = false;
}
