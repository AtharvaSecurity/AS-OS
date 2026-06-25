#include "packages.h"
#include "terminal.h"
#include "memory.h"

static package_t packages[MAX_PACKAGES];
static int package_count = 0;

void pkg_init() {
    package_count = 0;
    
    // Add available packages
    package_t pkgs[] = {
        {"terminal", "Terminal Emulator", "1.0", false, 1024},
        {"kate", "Text Editor", "2.0", false, 2048},
        {"chrome", "Web Browser", "1.0", false, 4096},
        {"files", "File Manager", "1.0", false, 1536},
        {"calculator", "Calculator App", "1.0", false, 512},
        {"settings", "System Settings", "1.0", false, 768},
        {"network", "Network Manager", "1.0", false, 2048},
        {"games", "Simple Games", "1.0", false, 3072},
        {"music", "Music Player", "1.0", false, 4096},
        {"paint", "Drawing App", "1.0", false, 2560}
    };
    
    for(int i = 0; i < 10; i++) {
        packages[package_count] = pkgs[i];
        package_count++;
    }
}

void pkg_install(const char* name) {
    for(int i = 0; i < package_count; i++) {
        if(strcmp(packages[i].name, name) == 0) {
            if(packages[i].installed) {
                terminal_writestring("  Already installed!\n");
                return;
            }
            packages[i].installed = true;
            terminal_writestring("  Installing ");
            terminal_writestring(name);
            terminal_writestring("...");
            for(int j = 0; j < 10; j++) {
                terminal_putchar('#');
                for(volatile int k = 0; k < 500000; k++) __asm__("nop");
            }
            terminal_writestring(" Done!\n");
            return;
        }
    }
    terminal_writestring("  Package not found!\n");
}

void pkg_remove(const char* name) {
    for(int i = 0; i < package_count; i++) {
        if(strcmp(packages[i].name, name) == 0) {
            if(!packages[i].installed) {
                terminal_writestring("  Not installed!\n");
                return;
            }
            packages[i].installed = false;
            terminal_writestring("  Removed ");
            terminal_writestring(name);
            terminal_writestring("!\n");
            return;
        }
    }
    terminal_writestring("  Package not found!\n");
}

void pkg_list() {
    terminal_writestring("\n  Package Manager\n");
    terminal_writestring("  ===============\n\n");
    terminal_writestring("  NAME        VERSION  STATUS      SIZE\n");
    terminal_writestring("  ----        -------  ------      ----\n");
    
    for(int i = 0; i < package_count; i++) {
        terminal_writestring("  ");
        terminal_writestring(packages[i].name);
        // Padding
        int len = strlen(packages[i].name);
        for(int j = len; j < 12; j++) terminal_putchar(' ');
        
        terminal_writestring(packages[i].version);
        for(int j = strlen(packages[i].version); j < 9; j++) terminal_putchar(' ');
        
        terminal_writestring(packages[i].installed ? "INSTALLED" : "available");
        for(int j = (packages[i].installed ? 9 : 9); j < 11; j++) terminal_putchar(' ');
        
        // Size
        char buf[8];
        int sz = packages[i].size;
        if(sz >= 1024) {
            buf[0] = '0' + (sz/1024);
            buf[1] = 'K';
            buf[2] = 'B';
            buf[3] = 0;
        } else {
            buf[0] = '0' + (sz/100);
            buf[1] = '0' + ((sz/10)%10);
            buf[2] = '0' + (sz%10);
            buf[3] = 'B';
            buf[4] = 0;
        }
        terminal_writestring(buf);
        terminal_writestring("\n");
    }
    terminal_writestring("\n");
}

void pkg_search(const char* query) {
    terminal_writestring("\n  Search results for: ");
    terminal_writestring(query);
    terminal_writestring("\n  =====================\n");
    for(int i = 0; i < package_count; i++) {
        // Simple search
        bool found = false;
        for(int j = 0; packages[i].name[j]; j++) {
            if(packages[i].name[j] == query[0]) found = true;
        }
        if(found) {
            terminal_writestring("  ");
            terminal_writestring(packages[i].name);
            terminal_writestring(" - ");
            terminal_writestring(packages[i].description);
            terminal_writestring("\n");
        }
    }
    terminal_writestring("\n");
}

void pkg_info(const char* name) {
    for(int i = 0; i < package_count; i++) {
        if(strcmp(packages[i].name, name) == 0) {
            terminal_writestring("\n  Package: ");
            terminal_writestring(packages[i].name);
            terminal_writestring("\n  Description: ");
            terminal_writestring(packages[i].description);
            terminal_writestring("\n  Version: ");
            terminal_writestring(packages[i].version);
            terminal_writestring("\n  Size: ");
            char buf[8];
            int sz = packages[i].size;
            buf[0] = '0' + (sz/1024);
            buf[1] = 'K';
            buf[2] = 'B';
            buf[3] = 0;
            terminal_writestring(buf);
            terminal_writestring("\n  Status: ");
            terminal_writestring(packages[i].installed ? "Installed" : "Not installed");
            terminal_writestring("\n\n");
            return;
        }
    }
    terminal_writestring("  Package not found!\n");
}
