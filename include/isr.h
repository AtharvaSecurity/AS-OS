#ifndef ISR_H
#define ISR_H

#include "stdint.h"
#include "stdbool.h"

// Register structure for interrupt handlers
typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

// Timer functions
void timer_init(uint32_t frequency);
uint32_t timer_get_ticks();
void sleep(uint32_t ms);

// IRQ functions
void irq_install_handler(int irq, void (*handler)(registers_t*));
void irq_uninstall_handler(int irq);
void irq_handler(registers_t* regs);

#endif // ISR_H
