#include "auth.h"
#include "memory.h"
#include "terminal.h"

static user_t users[MAX_USERS];
static int user_count = 0;
static user_t* current_user = 0;

void auth_init() {
    user_count = 0;
    current_user = 0;
    
    // Add default users
    auth_register("root", "asos2024", USER_ROOT);
    auth_register("atharva", "password", USER_ADMIN);
    auth_register("admin", "admin123", USER_ADMIN);
    auth_register("guest", "guest", USER_NORMAL);
}

bool auth_register(const char* username, const char* password, user_role_t role) {
    if(user_count >= MAX_USERS) return false;
    
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].username, username) == 0) return false;
    }
    
    for(int i = 0; i < 31 && username[i]; i++) {
        users[user_count].username[i] = username[i];
        users[user_count].username[i+1] = '\0';
    }
    for(int i = 0; i < 31 && password[i]; i++) {
        users[user_count].password[i] = password[i];
        users[user_count].password[i+1] = '\0';
    }
    
    users[user_count].role = role;
    users[user_count].logged_in = false;
    users[user_count].login_count = 0;
    user_count++;
    return true;
}

bool auth_login(const char* username, const char* password) {
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].username, username) == 0 && 
           strcmp(users[i].password, password) == 0) {
            users[i].logged_in = true;
            users[i].login_count++;
            current_user = &users[i];
            
            // Don't execute any shell commands here - just login
            return true;
        }
    }
    return false;
}

void auth_logout() {
    if(current_user) {
        current_user->logged_in = false;
        current_user = 0;
    }
}

user_t* auth_get_current_user() {
    return current_user;
}

void auth_list_users() {
    terminal_writestring("\n  Users:\n  ======\n");
    for(int i = 0; i < user_count; i++) {
        terminal_writestring("  ");
        terminal_writestring(users[i].username);
        terminal_writestring(" [");
        switch(users[i].role) {
            case USER_ROOT: terminal_writestring("ROOT"); break;
            case USER_ADMIN: terminal_writestring("ADMIN"); break;
            case USER_NORMAL: terminal_writestring("USER"); break;
        }
        terminal_writestring("]\n");
    }
    terminal_writestring("\n");
}
