#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class vga {

    /* Hardware text mode color constants. */
    enum vga_color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };
  
    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;
   
    size_t row;
    size_t column;
    uint8_t color;
    uint16_t* buffer;
   
    static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
    {
        return fg | bg << 4;
    }
     
    static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
    {
        return (uint16_t) uc | (uint16_t) color << 8;
    }
     
    size_t strlen(const char* str) 
    {
        size_t len = 0;
        while (str[len])
            len++;
        return len;
    }

    void nextline();
    void tab();
    void forward();
    
public:
    void initialize ();
    void setcolor(uint8_t color);
    void setCursorPosition(int row, int column);
    void putentryat(char c, uint8_t color, size_t x, size_t y);
    void putchar(char c);
    void write(const char* data, size_t size);
    void writestring(const char* data);
    void writeInt(const uint32_t value);
    void writeHex(const uint32_t value);

    };
