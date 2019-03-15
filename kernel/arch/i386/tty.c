/*
    

    Diese Datei ist Teil von NanOs.

    NanOs ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiter verteilen und/oder modifizieren.

    NanOs wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
*/
/*
|**********************************************************************;
* Project           : NanOs
*
* Author            : Peter Müller
*
* Date created      : 2019
*
|**********************************************************************;
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/baseIO.h>
#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

 
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;


void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
 
void terminal_disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void terminal_update_cursor(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}


void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_disable_cursor();
}
 

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}


void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
} 

void terminal_nextline()
{
    if (++terminal_row == VGA_HEIGHT)
    {
        // shift all lines one up
        for (size_t letter = VGA_WIDTH; letter < VGA_WIDTH * VGA_HEIGHT; letter++){
            terminal_buffer[letter-VGA_WIDTH] = terminal_buffer[letter];
            }
        // clear the last line
        for (size_t x=0; x < VGA_WIDTH; x++){
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
            }
            // row is now one above the bottom
            terminal_row--;
    }
    terminal_column=0;
} 

void terminal_forward()
{
    if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_nextline();
        }     
}


void _terminal_putchar(char c) {
    unsigned const char uc = (unsigned const char) c;
    switch (uc) {
    case '\n':
        terminal_nextline(); break;
//    case '\t':
        //tab(); break;
    default:
        terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
        terminal_forward();           
    } 
}

void terminal_putchar(char c) {
    _terminal_putchar(c);
    terminal_update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        _terminal_putchar(data[i]);
    terminal_update_cursor(terminal_column, terminal_row);
}


void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
    terminal_update_cursor(terminal_column, terminal_row);
}