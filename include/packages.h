#ifndef PACKAGES_H
#define PACKAGES_H

#include "stdint.h"
#include "stdbool.h"

#define MAX_PACKAGES 20
#define PKG_NAME_LEN 32
#define PKG_DESC_LEN 64

typedef struct {
    char name[PKG_NAME_LEN];
    char description[PKG_DESC_LEN];
    char version[16];
    bool installed;
    uint32_t size;
} package_t;

void pkg_init();
void pkg_install(const char* name);
void pkg_remove(const char* name);
void pkg_list();
void pkg_search(const char* query);
void pkg_info(const char* name);

#endif
