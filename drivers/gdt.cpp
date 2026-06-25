#include "gdt.h"
#include "terminal.h"

extern "C" {
    void gdt_flush(uint32_t);
    void idt_flush(uint32_t);
}

static gdt_entry gdt_entries[3];
static gdt_ptr gdt_ptr_info;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[num].access = access;
}

void gdt_init() {
    gdt_ptr_info.limit = (sizeof(gdt_entry) * 3) - 1;
    gdt_ptr_info.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    gdt_flush((uint32_t)&gdt_ptr_info);
}

void idt_init() {
    // Minimal IDT - just enough to not crash
}
