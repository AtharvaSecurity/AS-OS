#include "terminal.h"
#include "keyboard.h"
#include "cursor.h"
#include "gdt.h"
#include "isr.h"
#include "memory.h"
#include "process.h"
#include "fs.h"
#include "auth.h"
#include "network.h"
#include "gui.h"
#include "packages.h"
#include "mouse.h"
#include "graphics.h"

extern "C" {
    void outb(uint8_t value, uint16_t port);
    uint8_t inb(uint16_t port);
}
void calculator_app();

#define VGA ((uint16_t*)0xB8000)
#define BG  vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
#define CY  vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK)
#define GR  vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK)
#define RD  vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK)
#define YL  vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK)
#define MG  vga_entry_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK)
#define BL  vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK)
#define WH  vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
#define GRY vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY)
#define PAN vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY)
#define TIT vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE)

void c(int y, const char* s, uint8_t cl) {
    w((80-strlen(s))/2, y, s, cl);
}

void f(int x, int y, int w_, int h, char ch, uint8_t cl) {
    for(int r=y; r<y+h&&r<25; r++)
        for(int co=x; co<x+w_&&co<80; co++)
            p(co, r, ch, cl);
}

void h(int y, int x, int w_, char ch, uint8_t cl) {
    for(int i=0; i<w_&&x+i<80; i++)
        p(x+i, y, ch, cl);
}

void beep() {
    outb(0xB6,0x43);
    outb(0x1C,0x42);
    outb(0x00,0x42);
    uint8_t t=inb(0x61);
    outb(t|3,0x61);
    for(volatile int i=0;i<80000;i++) __asm__("nop");
    t=inb(0x61)&0xFC;
    outb(t,0x61);
}

void boot(){
    terminal_clear();terminal_hide_cursor();
    f(0,0,80,25,' ',BG);
    for(int i=0;i<80;i+=2){p(i,0,'=',CY);p(i,24,'=',CY);delay(5);}
    c(4,"+==========================================+",CY);
    c(5,"|              A S   O S                   |",CY);
    c(6,"|         KDE PLASMA v0.1                 |",CY);
    c(7,"+==========================================+",CY);
    const char*ph[]={"GDT/IDT","Memory","Timer","FS","Auth","Network","GUI","Packages","Ready!"};
    for(int i=0;i<9;i++){c(12+i,ph[i],i<8?GR:CY);delay(150);}
    beep();delay(400);
}

void login(){
    terminal_clear();terminal_hide_cursor();
    f(0,0,80,25,' ',BG);
    f(0,0,80,2,' ',TIT);c(0,"AS OS - KDE Plasma Login",WH);

    box(22,7,36,11,CY,BG);
    c(8,"WELCOME",CY);
    c(9,"Please login",WH);

    w(26,11,"User:",WH);f(32,11,22,1,' ',GRY);
    w(26,13,"Pass:",WH);f(32,13,22,1,' ',GRY);

    char un[32],pw[32];
    while(1){
        cursor_draw(32,11);
        int pos=0;
        for(int i=0;i<22;i++)VGA[11*80+32+i]=vga_entry(' ',GRY);
        while(1){
            keyboard_update();
            if(keyboard_has_char()){
                char c=keyboard_read_char();
                if(c=='\n'&&pos>0){un[pos]=0;break;}
                if(c=='\b'&&pos>0){pos--;VGA[11*80+32+pos]=vga_entry(' ',GRY);cursor_draw(32+pos,11);}
                if(pos<21&&c>=' '&&c!='\n'){un[pos]=c;VGA[11*80+32+pos]=vga_entry(c,GRY);pos++;cursor_draw(32+pos,11);}
            }
        }

        cursor_draw(32,13);pos=0;
        for(int i=0;i<22;i++)VGA[13*80+32+i]=vga_entry(' ',GRY);
        while(1){
            keyboard_update();
            if(keyboard_has_char()){
                char c=keyboard_read_char();
                if(c=='\n'&&pos>0){pw[pos]=0;break;}
                if(c=='\b'&&pos>0){pos--;VGA[13*80+32+pos]=vga_entry(' ',GRY);cursor_draw(32+pos,13);}
                if(pos<21&&c>=' '&&c!='\n'){pw[pos]=c;VGA[13*80+32+pos]=vga_entry('*',GRY);pos++;cursor_draw(32+pos,13);}
            }
        }

        cursor_hide();
        if(auth_login(un,pw)){
            c(15,"Access Granted!",GR);
            beep();
            delay(300);
            return;
        }
        else{
            c(15,"Invalid! Try again",RD);
            delay(600);
            f(22,15,36,1,' ',BG);
            // Clear the password field for retry
            f(32,13,22,1,' ',GRY);
        }
    }
}

void desktop(){
    terminal_clear();terminal_hide_cursor();

    for(int y=0;y<25;y++){
        vga_color shade=(y<6)?VGA_COLOR_BLUE:(y<12)?VGA_COLOR_MAGENTA:(y<18)?VGA_COLOR_CYAN:VGA_COLOR_GREEN;
        f(0,y,80,1,' ',vga_entry_color(VGA_COLOR_WHITE,shade));
    }

    for(int i=0;i<40;i++){int sx=(i*17+11)%80,sy=(i*13+7)%23+1;VGA[sy*80+sx]=vga_entry('.',WH);}

    f(0,0,80,1,' ',PAN);
    w(1,0,"AS OS",WH);w(10,0,"File",WH);w(16,0,"Edit",WH);w(22,0,"View",WH);
    c(0,"10:30 AM",WH);
    w(72,0,"^",RD);

    box(2,3,10,6,CY,BG);w(4,5,"Chr",CY);w(3,9,"Chrome",WH);
    box(16,3,10,6,GR,BG);w(18,5,"Ter",GR);w(16,9,"Terminal",WH);
    box(30,3,10,6,MG,BG);w(32,5,"Kat",MG);w(31,9,"Kate",WH);
    box(44,3,10,6,YL,BG);w(46,5,"Fil",YL);w(45,9,"Files",WH);
    box(58,3,10,6,BL,BG);w(60,5,"Net",BL);w(58,9,"Network",WH);
    box(68,3,10,6,RD,BG);w(69,5,"Sto",RD);w(68,9,"Store",WH);

    f(5,23,70,2,' ',PAN);
    c(23,"[1]Chrome [2]Term [3]Kate [4]Files [5]Net [6]Store [0]Out [Q]Quit",WH);

    c(14,"WELCOME TO AS OS",WH);
    c(16,"Press 1-6 to launch apps",CY);

    cursor_draw(40,16);
}

void app_chrome() {
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(1,1,78,23,CY,BG);f(2,1,77,1,' ',TIT);
    w(3,1,"Google Chrome - AS OS Edition",WH);w(61,1,"[X]",RD);

    f(3,3,74,1,' ',GRY);
    w(4,3,"[",WH);
    w(5,3,"https://",WH);

    cursor_draw(12,3);
    char url[50]; int pos=0;
    while(1) {
        keyboard_update();
        if(keyboard_has_char()) {
            char c = keyboard_read_char();
            if(c == '\n') {
                url[pos] = 0;
                break;
            }
            if(c == '\b' && pos > 0) {
                pos--;
                VGA[3*80+12+pos] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY));
                cursor_draw(12+pos, 3);
            }
            if(pos < 38 && c >= ' ' && c != '\n') {
                url[pos] = c;
                VGA[3*80+12+pos] = vga_entry(c, vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY));
                pos++;
                cursor_draw(12+pos, 3);
            }
        }
    }
    cursor_hide();
    w(75,3,"]",WH);

    w(4,5,"Loading: ",CY);
    w(14,5,url,YL);

    const char* spinners[] = {"|","/","-","\\"};
    for(int i=0; i<12; i++) {
        w(4,6,"[",WH);
        w(5,6,spinners[i%4],GR);
        w(6,6,"] Loading page...",WH);
        delay(100);
    }

    box(4,7,72,14,CY,BG);
    w(6,8,"AS OS Search Engine",WH);
    h(8,6,68,'-',CY);

    f(10,9,50,1,' ',GRY);
    w(11,9,"Search...",vga_entry_color(VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY));

    w(6,10,"Gmail   Images   YouTube   Maps   Drive",BL);
    w(6,11,"Search results for: ",CY);
    w(29,11,url,YL);

    w(6,12," 1. AS OS Official Website  - asos.com",BL);
    w(6,13," 2. AS OS Documentation Hub  - docs.asos.com",BL);
    w(6,14," 3. AS OS Community Forums   - forum.asos.com",BL);
    w(6,15," 4. AS OS GitHub Repository  - github.com/asos",BL);
    w(6,16," 5. AS OS Download Center    - download.asos.com",BL);

    w(52,10,"Latest News",RD);
    w(52,11,"v10.0 Released!",GR);
    w(52,12,"KDE Plasma UI",WH);
    w(52,13,"New Features",WH);
    w(52,14,"Bug Fixes",WH);
    w(52,15,"Community",WH);
    w(52,16,"Updates",WH);

    f(3,22,74,1,' ',PAN);
    w(4,22,"Secure | Connected | 2.3GB | v10.0",WH);

    w(4,19,"[Back]  [Forward]  [Reload]  [Bookmark]",WH);
    w(4,20,"[New Tab]  [History]  [Settings]  [Close]",WH);

    w(50,19,"[ESC]=Close",RD);
    w(50,20,"[R]=Reload",YL);

    while(1) {
        char c = keyboard_read_char();
        if(c == 27) break;
        if(c == 'r' || c == 'R') {
            w(4,6,"[",WH);
            w(5,6,"*",YL);
            w(6,6,"] Reloading...",YL);
            for(int i=0; i<8; i++) {
                w(5,6,".",WH);
                delay(100);
                w(5,6,"..",WH);
                delay(100);
                w(5,6,"...",WH);
                delay(100);
            }
            w(4,6,"[OK] Page reloaded!     ",GR);
            delay(300);
            w(4,6,"                        ",BG);
            w(5,6," ",BG);
            w(6,6," ",BG);
            w(4,6,"[",WH);
            w(5,6,"-",GR);
            w(6,6,"] Idle",WH);
        }
    }
}

void app_terminal(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(2,1,76,23,GR,BG);f(3,1,75,1,' ',TIT);
    w(4,1,"Konsole Terminal",WH);w(65,1,"[X]",RD);
    terminal_setcolor(GR);
    terminal_writestring("\n  AS OS Terminal v10.0\n  =====================\n  Type 'exit' to close\n\n");
    char cmd[256];
    while(1){
        terminal_setcolor(GR);terminal_writestring("  $ ");terminal_show_cursor();
        keyboard_read_line(cmd,256);terminal_hide_cursor();
        if(strcmp(cmd,"exit")==0)break;
        else if(strcmp(cmd,"help")==0)terminal_writestring("  help exit clear ls ps mem users net ping pkg date time whoami neofetch calc\n");
        else if(strcmp(cmd,"clear")==0){terminal_clear();f(0,0,80,25,' ',BG);box(2,1,76,23,GR,BG);f(3,1,75,1,' ',TIT);w(4,1,"Konsole Terminal",WH);}
        else if(strcmp(cmd,"ls")==0)fs_list_files();
        else if(strcmp(cmd,"ps")==0)process_list();
        else if(strcmp(cmd,"mem")==0)memory_info();
        else if(strcmp(cmd,"users")==0)auth_list_users();
        else if(strcmp(cmd,"net")==0)network_status();
        else if(strncmp(cmd,"ping ",5)==0)network_ping(cmd+5);
        else if(strcmp(cmd,"pkg")==0)pkg_list();
        else if(strncmp(cmd,"install ",8)==0)pkg_install(cmd+8);
        else if(strncmp(cmd,"search ",7)==0)pkg_search(cmd+7);
        else if(strcmp(cmd,"date")==0)terminal_writestring("  January 15, 2024\n");
        else if(strcmp(cmd,"time")==0)terminal_writestring("  10:30:00 AM\n");
        else if(strcmp(cmd,"whoami")==0){user_t*cu=auth_get_current_user();terminal_writestring("  ");terminal_writestring(cu?cu->username:"none");terminal_writestring("\n");}
        else if(strcmp(cmd,"neofetch")==0){
            terminal_writestring("      .--.        OS: AS OS 10.0 KDE\n");
            terminal_writestring("     |o_o |       DE: AS CHSMA\n");
            terminal_writestring("     |:_/ |       WM: AWin\n");
            terminal_writestring("    //   \\ \\      Theme: MALT\n");
            terminal_writestring("   (|     | )     CPU: x86 62-bit\n");
            terminal_writestring("  /'\\_   _/'\\    RAM: 15.4GB\n");
            terminal_writestring("  \\___)=(___/     \n\n");
        }
        else if(strcmp(cmd,"calc")==0)calculator_app();
        else if(strlen(cmd)>0){terminal_writestring("  ");terminal_writestring(cmd);terminal_writestring(": not found\n");}
    }
}

void app_kate(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(2,1,76,23,MG,BG);f(3,1,75,1,' ',TIT);
    w(4,1,"Kate Editor",WH);w(55,1,"'xx'=save [X]",RD);
    terminal_setcolor(WH);
    terminal_writestring("\n  Kate Text Editor\n  Start typing...\n\n  ");
    terminal_show_cursor();
    int p=0,xc=0;
    while(p<2000){char c=keyboard_read_char();if(c=='x'||c=='X'){xc++;if(xc>=2)break;terminal_putchar(c);p++;continue;}xc=0;if(c=='\n'){terminal_putchar('\n');terminal_writestring("  ");p++;}else if(c=='\b'&&p>0){p--;terminal_putchar('\b');}else if(c>=' '){terminal_putchar(c);p++;}}
    terminal_hide_cursor();c(21,"File saved!",GR);delay(500);
}

void app_files(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(2,1,76,23,YL,BG);f(3,1,75,1,' ',TIT);
    w(4,1,"Dolphin File Manager",WH);w(55,1,"Any key=close",RD);
    f(3,2,15,20,' ',GRY);
    const char*d[]={"Home","Documents","Downloads","Pictures","Music","Videos","System"};
    for(int i=0;i<7;i++)w(5,3+i,d[i],vga_entry_color(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_GREY));
    w(20,3,"Files",CY);h(4,20,55,'-',CY);
    w(20,5,"readme.txt   1.0KB   Jan15",WH);
    w(20,6,"resume.pdf   2.3MB   Jan12",WH);
    w(20,7,"notes.txt     856B   Jan14",WH);
    w(20,8,"project.c    1.2KB   Jan15",WH);
    c(21,"Press any key to close",RD);
    keyboard_read_char();
}

void app_network(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(5,3,70,19,BL,BG);f(6,3,69,1,' ',TIT);
    w(7,3,"Network Center",WH);w(55,3,"[X]Close",RD);
    w(9,5,"WiFi: Connected",GR);
    w(9,6,"IP: 192.168.1.100",WH);
    w(9,7,"Gateway: 192.168.1.1",WH);
    w(9,8,"DNS: 8.8.8.8",WH);
    w(9,10,"Available Networks:",CY);
    w(9,11,"AS-Network [Connected]",GR);
    w(9,12,"Guest-WiFi [Open]",WH);
    w(9,13,"Public-Hotspot [Secured]",WH);
    while(1){char c=keyboard_read_char();if(c=='x'||c=='X')break;}
}

void app_store(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(2,1,76,23,GR,BG);f(3,1,75,1,' ',TIT);
    w(4,1,"Discover App Store",WH);w(55,1,"[X]Close",RD);
    const char*apps[]={"Terminal Pro","Chrome Browser","Kate Editor","File Explorer","System Monitor","Network Center","Calculator","Clock App","Games Pack","Music Player"};
    for(int i=0;i<10;i++){w(5,4+i,apps[i],WH);w(40,4+i,"[INSTALL]",GR);}
    w(5,16,"Type 'install <name>' in Terminal to install",CY);
    while(1){char c=keyboard_read_char();if(c=='x'||c=='X')break;}
}

void app_system(){
    terminal_clear();terminal_hide_cursor();f(0,0,80,25,' ',BG);
    box(3,1,74,23,RD,BG);f(4,1,73,1,' ',TIT);
    w(5,1,"System Settings",WH);w(55,1,"[X]Close",RD);
    w(7,4,"AS OS v10.0 KDE Plasma Edition",CY);
    h(5,7,66,'-',CY);
    w(7,6,"Kernel: ASKernel 10.0",WH);
    w(7,7,"Desktop: KDE Plasma (Simulated)",BL);
    w(7,8,"Theme: Breeze Dark",WH);
    w(7,9,"CPU: x86 32-bit | RAM: 1MB",WH);
    w(7,10,"Display: VGA 80x25 Text Mode",WH);
    h(12,7,66,'-',CY);
    w(7,13,"ALL 10 PHASES COMPLETE!",GR);
    w(7,14,"GDT+IDT+Memory+Timer+FS+Auth+Network+GUI+Packages",GR);
    while(1){char c=keyboard_read_char();if(c=='x'||c=='X')break;}
}

void desktop_loop(){
    while(1){
        desktop();
        char key=keyboard_read_char();
        switch(key){
            case '1':app_chrome();break;
            case '2':app_terminal();break;
            case '3':app_kate();break;
            case '4':app_files();break;
            case '5':app_network();break;
            case '6':app_store();break;
            case '7':app_system();break;
            case '8':calculator_app();break;
            case '0':auth_logout();return;
            case 'q':case 'Q':
                terminal_clear();f(0,0,80,25,' ',BG);
                c(12,"Shutting Down...",RD);
                while(1)__asm__("hlt");
        }
    }
}

extern "C" void kernel_main(){
    terminal_initialize();
    keyboard_initialize();
    cursor_init();
    gdt_init();
    idt_init();
    memory_init();
    timer_init(1000);
    scheduler_init();
    fs_init();
    auth_init();
    network_init();
    gui_init();
    pkg_init();
    mouse_init();
    terminal_hide_cursor();

    boot();
    while(1) {
        login();
        while(1) {
            desktop();
            gui_update();
            if(keyboard_has_char()) {
                char key = keyboard_read_char();
                (void)key;
            }
            delay(10);
        }
    }
}
