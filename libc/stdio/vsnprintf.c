


#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool __vsnprintf(char* restrict string, size_t start, size_t n,const char* data, size_t length) {
    
    size_t i = 0;
    for ( ; (i < length) && (start+i+1<n) ; i++)
        string[start+i] = data[i];
        
    if (start+i+1>=n)
        string[n] = (char) 0;
        
    return true;
}

int vsnprintf(char* restrict string, size_t n, const char* restrict format, va_list parameters) {

    int written = 0;

    while (*format != '\0') {

        size_t maxrem = INT_MAX - written;
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (! __vsnprintf(string, written, n, format, amount))
                return -1;

            format += amount;
            written += amount;

            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!__vsnprintf(string, written, n, &c, sizeof(c)))
                return -1;
            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!__vsnprintf(string, written, n, str, len))
                return -1;
            written += len;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!__vsnprintf(string, written, n, format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    char end = '\0';
    __vsnprintf(string,written, n, &end, sizeof(end));
    
    return written;
}