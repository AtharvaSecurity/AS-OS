#ifndef GUI_EFFECTS_H
#define GUI_EFFECTS_H

#include "gui.h"

// ============================================================
// PARTICLE SYSTEMS
// ============================================================

typedef struct {
    int x, y;
    int vx, vy;
    int life;
    int max_life;
    uint8_t color;
    int size;
} particle_t;

typedef struct {
    particle_t particles[100];
    int count;
    bool active;
    int x, y;
} particle_system_t;

void particle_system_init(particle_system_t* ps, int x, int y);
void particle_system_update(particle_system_t* ps);
void particle_system_draw(particle_system_t* ps);
void particle_system_burst(particle_system_t* ps, int count, int speed);
void particle_system_explode(particle_system_t* ps, int x, int y, int count);

// ============================================================
// FIRE & FIREWORKS
// ============================================================

void gui_firework_effect(int x, int y, int color);
void gui_sparkle_effect(int x, int y, int duration_ms);
void gui_star_effect(int x, int y, int size);
void gui_confetti_effect(int x, int y, int count);

// ============================================================
// TRANSITIONS
// ============================================================

typedef enum {
    TRANSITION_FADE,
    TRANSITION_SLIDE_LEFT,
    TRANSITION_SLIDE_RIGHT,
    TRANSITION_SLIDE_UP,
    TRANSITION_SLIDE_DOWN,
    TRANSITION_ZOOM_IN,
    TRANSITION_ZOOM_OUT,
    TRANSITION_FLIP,
    TRANSITION_BOUNCE,
    TRANSITION_WIPE
} transition_type_t;

void gui_transition_start(transition_type_t type, int duration_ms);
void gui_transition_update();
void gui_transition_draw();

// ============================================================
// 3D EFFECTS
// ============================================================

void gui_draw_3d_box(int x, int y, int w, int h, int depth, uint8_t color1, uint8_t color2);
void gui_draw_perspective_cube(int cx, int cy, int size);
void gui_draw_parallax_background(int offset_x, int offset_y);

// ============================================================
// SPECIAL EFFECTS
// ============================================================

void gui_glass_effect(int x, int y, int w, int h);
void gui_neon_glow(int x, int y, const char* text, uint8_t color);
void gui_scanline_effect();
void gui_vignette_effect();
void gui_rain_effect();
void gui_snow_effect();
void gui_fire_effect(int x, int y, int w, int h);
void gui_water_effect(int x, int y, int w, int h);
void gui_aurora_effect(int x, int y, int w, int h);

// ============================================================
// ANIMATION HELPERS
// ============================================================

typedef struct {
    float value;
    float target;
    float speed;
    bool looping;
    float min_val;
    float max_val;
    float (*easing_func)(float t);
} animator_t;

void animator_init(animator_t* anim, float start, float target, float speed);
void animator_update(animator_t* anim);
float animator_get_value(animator_t* anim);
float animator_lerp(float a, float b, float t);
float animator_ease_in(float t);
float animator_ease_out(float t);
float animator_ease_in_out(float t);
float animator_bounce(float t);
float animator_elastic(float t);
float animator_back(float t);

// ============================================================
// COLOR UTILITIES
// ============================================================

uint8_t gui_color_blend(uint8_t color1, uint8_t color2, float t);
uint8_t gui_color_lighten(uint8_t color, int amount);
uint8_t gui_color_darken(uint8_t color, int amount);
uint8_t gui_color_alpha(uint8_t color, float alpha);
uint8_t gui_color_rainbow(float time);
uint8_t gui_color_heatmap(float value);

#endif // GUI_EFFECTS_H
