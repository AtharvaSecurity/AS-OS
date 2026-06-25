#include "process.h"
#include "terminal.h"
#include "memory.h"

static process_t* process_table[MAX_PROCESSES];
static int process_count = 0;

void scheduler_init() {
    for(int i = 0; i < MAX_PROCESSES; i++) process_table[i] = 0;
    process_count = 0;
}

process_t* process_create(const char* name, void (*entry)()) {
    if(process_count >= MAX_PROCESSES) return 0;
    
    process_t* proc = (process_t*)malloc(sizeof(process_t));
    if(!proc) return 0;
    
    proc->pid = process_count + 1;
    proc->state = PROCESS_READY;
    proc->entry = entry;
    
    for(int i = 0; i < 31 && name[i]; i++) {
        proc->name[i] = name[i];
        proc->name[i+1] = '\0';
    }
    
    process_table[process_count++] = proc;
    return proc;
}

void process_list() {
    terminal_writestring("\n  Processes:\n  ==========\n");
    for(int i = 0; i < process_count; i++) {
        if(process_table[i]) {
            terminal_writestring("  PID ");
            char buf[8];
            int pid = process_table[i]->pid;
            buf[0] = '0' + pid;
            buf[1] = ':';
            buf[2] = ' ';
            buf[3] = 0;
            terminal_writestring(buf);
            terminal_writestring(process_table[i]->name);
            terminal_writestring("\n");
        }
    }
    terminal_writestring("\n");
}
