#ifndef MOUSE_H
#define MOUSE_H

#include "stdint.h"
#include "stdbool.h"
#include "isr.h"

typedef struct {
    int x, y;
    int last_x, last_y;
    bool left_button;
    bool right_button;
    bool middle_button;
    bool left_clicked;
    bool right_clicked;
    bool left_released;
    int wheel_delta;
    bool moved;
} mouse_state_t;

#ifdef __cplusplus
extern "C" {
#endif

void mouse_init();
void mouse_update();
mouse_state_t* mouse_get_state();
void mouse_set_position(int x, int y);
void mouse_show();
void mouse_hide();
void mouse_draw();

#ifdef __cplusplus
}
#endif

#endif // MOUSE_H
