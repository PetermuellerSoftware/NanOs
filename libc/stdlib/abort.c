#include <stdlib.h>
#include <stdio.h>

void abort() {
#if defined(__is_libk)
    printf ("kernel: panic: abort\n");
    __asm__ volatile (
        "cli;"
        "hlt_abort:;"
        "hlt;"
        "jmp hlt_abort;"
    );
#else
    printf ("abort\n");
    // TODO: sigabort handling
#endif
    
}