#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H 1

enum LOG_TYPE {
    LOG_TYPE_CRITICAL = 1,
    LOG_TYPE_ERROR = 2,
    LOG_TYPE_WARNING = 3,
    LOG_TYPE_INFO = 4,
    LOG_TYPE_DEBUG = 5,    
    LOG_TYPE_VERBOSE = 6    
};

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_TYPE_DEBUG
#endif

void log(int level, const char* msg);
void logf(int level, const char* format, ...);

#endif