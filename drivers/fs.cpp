#include "fs.h"
#include "memory.h"
#include "terminal.h"

static file_t files[MAX_FILES];
static int file_count = 0;
static dir_t dirs[MAX_DIRS];
static int dir_count = 0;

void fs_init() {
    file_count = 0;
    dir_count = 0;
    
    fs_create_dir("home");
    fs_create_dir("home/documents");
    fs_create_dir("home/downloads");
    fs_create_dir("home/pictures");
    fs_create_dir("home/music");
    fs_create_dir("home/videos");
    fs_create_dir("system");
}

file_t* fs_create_file(const char* name, const char* owner) {
    if(file_count >= MAX_FILES) return 0;
    
    for(int i = 0; i < 31 && name[i]; i++) {
        files[file_count].name[i] = name[i];
        files[file_count].name[i+1] = '\0';
    }
    for(int i = 0; i < 31 && owner[i]; i++) {
        files[file_count].owner[i] = owner[i];
        files[file_count].owner[i+1] = '\0';
    }
    
    files[file_count].type = FT_FILE;
    files[file_count].size = 0;
    files[file_count].data = (uint8_t*)malloc(MAX_FILE_SIZE);
    
    return &files[file_count++];
}

file_t* fs_open_file(const char* name) {
    for(int i = 0; i < file_count; i++) {
        if(strcmp(files[i].name, name) == 0) return &files[i];
    }
    return 0;
}

int fs_write_file(file_t* file, const uint8_t* data, uint32_t size) {
    if(!file || size > MAX_FILE_SIZE) return -1;
    for(uint32_t i = 0; i < size; i++) file->data[i] = data[i];
    file->size = size;
    return size;
}

void fs_list_files() {
    terminal_writestring("\n  Files:\n  ======\n");
    for(int i = 0; i < file_count; i++) {
        terminal_writestring("  ");
        terminal_writestring(files[i].name);
        terminal_writestring("\n");
    }
    
    terminal_writestring("\n  Directories:\n  ============\n");
    for(int i = 0; i < dir_count; i++) {
        terminal_writestring("  ");
        terminal_writestring(dirs[i].name);
        terminal_writestring("/\n");
    }
    terminal_writestring("\n");
}

dir_t* fs_create_dir(const char* name) {
    if(dir_count >= MAX_DIRS) return 0;
    for(int i = 0; i < 31 && name[i]; i++) {
        dirs[dir_count].name[i] = name[i];
        dirs[dir_count].name[i+1] = '\0';
    }
    return &dirs[dir_count++];
}

dir_t* fs_get_dir(const char* dirname) {
    for(int i = 0; i < dir_count; i++) {
        if(strcmp(dirs[i].name, dirname) == 0) return &dirs[i];
    }
    return 0;
}

void fs_list_dir(const char* dirname) {
    terminal_writestring("\n  Directory: ");
    terminal_writestring(dirname);
    terminal_writestring("\n");
    for(int i = 0; i < dir_count; i++) {
        terminal_writestring("  ");
        terminal_writestring(dirs[i].name);
        terminal_writestring("/\n");
    }
}
