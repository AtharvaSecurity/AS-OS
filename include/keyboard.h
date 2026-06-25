#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"
#include "stdbool.h"

void keyboard_initialize(void);
bool keyboard_has_char(void);
char keyboard_read_char(void);
void keyboard_update(void);
void keyboard_read_line(char* buffer, int max_len);

#endif
