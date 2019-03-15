#include <stdio.h>
#include <limits.h>

int vsprintf(char* restrict string, const char* restrict format, va_list parameters) {
    vsnprintf(string, INT_MAX, format, parameters);    
}