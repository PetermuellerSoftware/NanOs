#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)
 
#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char* __restrict, ...);
int sprintf(char* __restrict string, const char* __restrict, ...);
int snprintf(char* __restrict string, size_t n, const char* __restrict, ...);
int putchar(int);
int puts(const char*);

int vsnprintf(char* __restrict string, size_t n, const char* __restrict, va_list parameters);
int vsprintf(char* __restrict string, const char* __restrict, va_list parameters);
 
#ifdef __cplusplus
}
#endif
 
#endif