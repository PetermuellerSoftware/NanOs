#include <string.h>


void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t num) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src= (const unsigned char*) srcptr;
    for (size_t i = 0; i < num; i++) {
        dst[i] = src[i];
    }
    return dstptr;
}