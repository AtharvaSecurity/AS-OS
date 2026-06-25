#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "stdint.h"

typedef struct {
    double result;
    char operation;
    bool has_error;
} calc_t;

void calc_init(calc_t* calc);
void calc_input(calc_t* calc, double num);
void calc_operation(calc_t* calc, char op);
double calc_get_result(calc_t* calc);
void calc_clear(calc_t* calc);

#endif
