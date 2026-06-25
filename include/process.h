#ifndef PROCESS_H
#define PROCESS_H

#include "stdint.h"

#define MAX_PROCESSES 10
#define PROCESS_STACK_SIZE 4096

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state;

typedef struct {
    uint32_t pid;
    char name[32];
    process_state state;
    uint32_t* stack;
    uint32_t stack_size;
    uint32_t priority;
    uint32_t ticks;
    void (*entry)();
} process_t;

void scheduler_init();
process_t* process_create(const char* name, void (*entry)());
void process_exit();
void scheduler_run();
void process_list();

#endif
