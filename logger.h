#ifndef H_LOGGER
#define H_LOGGER
#define LOG(...) Logger_print(__FILE__, __LINE__, __VA_ARGS__ )

#include <stdio.h>
#include "semaphore.h"

struct Logger
{
    FILE *fp;
    struct Semaphore* sem;
};

struct Logger* Logger_get();

void Logger_init();

void Logger_print(char* filename, int line, char *fmt,...);

#endif
