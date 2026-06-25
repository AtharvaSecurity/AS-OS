#ifndef AUTH_H
#define AUTH_H

#include "stdint.h"
#include "stdbool.h"

#define MAX_USERS 10
#define MAX_USERNAME 32
#define MAX_PASSWORD 32

typedef enum {
    USER_NORMAL,
    USER_ADMIN,
    USER_ROOT
} user_role_t;

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    user_role_t role;
    bool logged_in;
    uint32_t last_login;
    uint32_t login_count;
} user_t;

void auth_init();
bool auth_register(const char* username, const char* password, user_role_t role);
bool auth_login(const char* username, const char* password);
void auth_logout();
user_t* auth_get_current_user();
user_t* auth_get_user(const char* username);
void auth_list_users();
bool auth_delete_user(const char* username);
bool auth_change_password(const char* username, const char* old_pass, const char* new_pass);

#endif
