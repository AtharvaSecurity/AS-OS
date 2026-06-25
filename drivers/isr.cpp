#include "isr.h"
#include "terminal.h"

extern "C" {
    void outb(uint8_t value, uint16_t port);
}

// IRQ handler array
static void (*irq_handlers[16])(registers_t*) = {0};

void timer_init(uint32_t frequency) {
    // Don't actually set up PIT yet - it can cause crashes
    // Just initialize tick counter
    (void)frequency; // Suppress unused parameter warning
}

uint32_t timer_get_ticks() {
    return 0;
}

void sleep(uint32_t ms) {
    // Simple busy-wait sleep
    for(volatile uint32_t i = 0; i < ms * 10000; i++) {
        __asm__ volatile("nop");
    }
}

void irq_install_handler(int irq, void (*handler)(registers_t*)) {
    if(irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_uninstall_handler(int irq) {
    if(irq >= 0 && irq < 16) {
        irq_handlers[irq] = 0;
    }
}

// IRQ handler function (called from assembly)
void irq_handler(registers_t* regs) {
    int irq = regs->int_no - 32;
    if(irq >= 0 && irq < 16) {
        if(irq_handlers[irq]) {
            irq_handlers[irq](regs);
        }
    }
    // Send EOI
    if(irq >= 8) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}
