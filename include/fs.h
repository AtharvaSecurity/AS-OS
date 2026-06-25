#ifndef FS_H
#define FS_H

#include "stdint.h"
#include "stddef.h"

#define MAX_FILES 64
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 4096
#define MAX_DIRS 16
#define MAX_DIR_NAME 32

// File types
typedef enum {
    FT_FILE,
    FT_DIRECTORY
} file_type_t;

// File structure
typedef struct {
    char name[MAX_FILENAME];
    file_type_t type;
    uint32_t size;
    uint8_t* data;
    uint32_t created_time;
    uint32_t modified_time;
    uint8_t permissions;  // rwx (read=4, write=2, execute=1)
    char owner[32];
} file_t;

// Directory structure
typedef struct {
    char name[MAX_DIR_NAME];
    file_t* files[MAX_FILES];
    int file_count;
} dir_t;

// Filesystem
void fs_init();
file_t* fs_create_file(const char* name, const char* owner);
file_t* fs_open_file(const char* name);
int fs_write_file(file_t* file, const uint8_t* data, uint32_t size);
int fs_read_file(file_t* file, uint8_t* buffer, uint32_t size);
int fs_delete_file(const char* name);
void fs_list_files();
dir_t* fs_create_dir(const char* name);
dir_t* fs_get_dir(const char* name);
void fs_list_dir(const char* dirname);

#endif
