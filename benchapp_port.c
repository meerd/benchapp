#include "benchapp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void benchapp_printf(const char* fmt, ... )
{
    va_list arglist;

    va_start(arglist, fmt);
    vprintf(fmt, arglist);

    va_end(arglist);
}

void* benchapp_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr) {
        memset(ptr, 0x00, size);
    }

    return ptr;
}
