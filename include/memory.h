#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"
#include "stddef.h"

#define MEMORY_SIZE 0x1000000  // 16MB
#define BLOCK_SIZE 4096
#define BLOCK_COUNT (MEMORY_SIZE / BLOCK_SIZE)

void memory_init();
void* malloc(size_t size);
void free(void* ptr);
void memory_info();
size_t memory_used();
size_t memory_free();

#endif
