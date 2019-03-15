#include <kernel/serial.h>
#include <string.h>

void serial_writestring(const char* string) {
    serial_write(string, strlen(string));
}