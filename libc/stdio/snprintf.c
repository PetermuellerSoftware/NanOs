#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int snprintf(char* restrict string, size_t n, const char* restrict format, ...) {

    va_list parameters;
    va_start(parameters, format);

    int written = vsnprintf( string, n, format, parameters);
    
    va_end(parameters);
    return written;
}