#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/log.h>
#include <kernel/serial.h>
#include <kernel/tty.h>


//	extern "C" 
void kernel_main(void) 
{    
    terminal_initialize();
    serial_initialize( SERIAL_BAUD_RATE_57600);

    // give the fist sign of live to the user
    log (LOG_TYPE_INFO, "NanOs is starting!\n\n");
            
}

