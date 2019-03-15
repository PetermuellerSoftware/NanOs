#include <stdio.h>
#include <limits.h>

int sprintf( char* restrict string, const char* restrict format, ...) {
     va_list parameters;
    va_start(parameters, format);

    int written = vsnprintf( string, INT_MAX, format, parameters);
    
    va_end(parameters);
    return written;
}