#include <kernel/log.h>

#include <stdarg.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>


void logf(int level, const char* format, ...) {
    char logmsg[255];
   
    va_list param;
    va_start(param, format);
   
    vsnprintf(logmsg , 255, format, param);

    va_end(param);
    
    log (level, logmsg);

}


void log(int level, const char* msg) {
    char log_entry[255];
    char clevel;
    terminal_writestring(msg);
    
    switch(level) {
        case LOG_TYPE_CRITICAL:
            clevel='C'; break;
        case LOG_TYPE_ERROR:
            clevel='E'; break;
        case LOG_TYPE_WARNING:
            clevel='W'; break;
        case LOG_TYPE_INFO:
            clevel='I'; break;
        case LOG_TYPE_DEBUG:
            clevel='D'; break;
        case LOG_TYPE_VERBOSE:
            clevel='V'; break;
        default:
            clevel=' '; break;
        }
    
    snprintf(log_entry, 255, "%c; %s", clevel, msg);
    serial_writestring(log_entry);
}
