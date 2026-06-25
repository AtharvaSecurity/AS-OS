#include "calculator.h"
#include "terminal.h"
#include "keyboard.h"

void calc_init(calc_t* calc) {
    calc->result = 0;
    calc->operation = 0;
    calc->has_error = false;
}

void calc_input(calc_t* calc, double num) {
    switch(calc->operation) {
        case '+': calc->result += num; break;
        case '-': calc->result -= num; break;
        case '*': calc->result *= num; break;
        case '/': 
            if(num != 0) calc->result /= num;
            else calc->has_error = true;
            break;
        default: calc->result = num; break;
    }
    calc->operation = 0;
}

void calc_operation(calc_t* calc, char op) {
    calc->operation = op;
}

double calc_get_result(calc_t* calc) {
    return calc->result;
}

void calc_clear(calc_t* calc) {
    calc->result = 0;
    calc->operation = 0;
    calc->has_error = false;
}

void calculator_app() {
    terminal_clear();
    terminal_hide_cursor();
    
    uint16_t* vga = (uint16_t*)0xB8000;
    uint8_t bg = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    uint8_t cyan = vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    uint8_t white = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    uint8_t green = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    // Background
    for(int y=0;y<25;y++) for(int x=0;x<80;x++) vga[y*80+x]=vga_entry(' ',bg);
    
    // Calculator window
    for(int r=4;r<20;r++) for(int c=25;c<55;c++) vga[r*80+c]=vga_entry(' ',vga_entry_color(VGA_COLOR_BLACK,VGA_COLOR_DARK_GREY));
    for(int i=26;i<54;i++){vga[4*80+i]=vga_entry('-',cyan);vga[19*80+i]=vga_entry('-',cyan);}
    for(int i=5;i<19;i++){vga[i*80+25]=vga_entry('|',cyan);vga[i*80+54]=vga_entry('|',cyan);}
    
    // Title
    const char* title = "CALCULATOR";
    int tx = (80-10)/2;
    for(int i=0;title[i];i++) vga[4*80+tx+i]=vga_entry(title[i],cyan);
    
    calc_t calc;
    calc_init(&calc);
    double current = 0;
    bool entering = false;
    
    while(1) {
        terminal_writestring("\n\n\n\n\n  Calculator (press 'q' to quit)\n  ================================\n  ");
        terminal_writestring("Result: ");
        
        char buf[32];
        int r = (int)calc_get_result(&calc);
        int pos = 0;
        if(r < 0) { buf[pos++] = '-'; r = -r; }
        if(r == 0) buf[pos++] = '0';
        else {
            char tmp[16]; int j = 0;
            while(r > 0) { tmp[j++] = '0' + (r % 10); r /= 10; }
            while(j > 0) buf[pos++] = tmp[--j];
        }
        buf[pos] = '\0';
        terminal_writestring(buf);
        terminal_writestring("\n\n  Operations: + - * / c(clear)");
        
        char c = keyboard_read_char();
        if(c == 'q' || c == 'Q') break;
        else if(c == 'c' || c == 'C') calc_clear(&calc);
        else if(c == '+' || c == '-' || c == '*' || c == '/') calc_operation(&calc, c);
        else if(c >= '0' && c <= '9') calc_input(&calc, (double)(c - '0'));
    }
}
