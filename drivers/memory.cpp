#include "memory.h"
#include "terminal.h"

static uint8_t memory_pool[1024 * 1024]; // 1MB heap
static size_t memory_offset = 0;

void memory_init() {
    memory_offset = 0;
    for(size_t i = 0; i < sizeof(memory_pool); i++) {
        memory_pool[i] = 0;
    }
}

void* malloc(size_t size) {
    if(memory_offset + size > sizeof(memory_pool)) return 0;
    void* ptr = &memory_pool[memory_offset];
    memory_offset += size;
    return ptr;
}

void free(void* ptr) {
    // Simple allocator - no free support needed yet
    (void)ptr;
}

size_t memory_used() {
    return memory_offset;
}

size_t memory_free() {
    return sizeof(memory_pool) - memory_offset;
}

void memory_info() {
    terminal_writestring("\n  Memory: 1MB heap\n");
    terminal_writestring("  Used: ");
    size_t used = memory_offset;
    char buf[16];
    int i = 0;
    if(used == 0) buf[i++] = '0';
    else {
        char tmp[16]; int j = 0;
        size_t temp = used;
        while(temp > 0) { tmp[j++] = '0' + (temp % 10); temp /= 10; }
        while(j > 0) buf[i++] = tmp[--j];
    }
    buf[i] = 0;
    terminal_writestring(buf);
    terminal_writestring(" bytes\n\n");
}
