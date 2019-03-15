#ifndef STRING_H
#define STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

int memcmp (const void * , const void * , size_t );
void* memcpy(void* __restrict, const void* __restrict, size_t);
void * memmove ( void * , const void * , size_t );
void * memset ( void * , int value, size_t );

size_t strlen ( const char * str );

#ifdef __cplusplus
}
#endif

#endif