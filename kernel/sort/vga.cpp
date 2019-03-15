#include "vga.h"
#include "math.h"





void vga::initialize() 
{
    this->row = 0;
	this->column = 0;
	this->color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	this->buffer = (uint16_t*) 0xB8000;
	
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			buffer[index] = vga_entry(' ', color);
		}
	}
}

void vga::setcolor(uint8_t color) 
{
	this->color = color;
}
 
void vga::putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	buffer[index] = vga_entry(c, color);
}
 
void vga::putchar(char c) 
{
    switch (c) {
    case '\n':
        nextline(); break;
    case '\t':
        tab(); break;
    default:
        putentryat(c, color, column, row);
        forward();           
    }    
}
 
void vga::nextline()
{
    if (++row == VGA_HEIGHT)
    {
        // shift all lines one up
        for (size_t letter = VGA_WIDTH; letter < VGA_WIDTH * VGA_HEIGHT; letter++){
            buffer[letter-VGA_WIDTH] = buffer[letter];
            }
        // clear the last line
        for (size_t x=0; x < VGA_WIDTH; x++){
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            buffer[index] = vga_entry(' ', color);
            }
            // row is now one above the bottom
            row--;
    }
    column=0;
} 

void vga::setCursorPosition(int row, int column)
{
    this->column = limit(0, (int) VGA_WIDTH - 1, column);
    this->row = limit(0, (int) VGA_HEIGHT - 1, row);    
}


void vga::forward()
{
    if (++column == VGA_WIDTH) {
            column = 0;
            nextline();
        }     
}

void vga::tab()
{
    const int amount = 4 - ( column % 4 );
    for (int i=0; i<amount; i++) {
        forward(); }
}
 
void vga::write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		this->putchar(data[i]);
}
 
void vga::writestring(const char* data) 
{
	this->write(data, strlen(data));
}

void vga::writeInt(const uint32_t value){
    char str[20];
    itoa( (int) value, str, 10);
    this->writestring(str);
}

void vga::writeHex(const uint32_t value){
    char str[20];
    itoa( (int) value, str, 16);
    this->writestring(str);
}