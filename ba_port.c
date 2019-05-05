#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "ba_types.h"

/** ***************** string **************************/

void ba_memset(void *dest, int ch, size_t size)
{
    unsigned char *p = (unsigned char *) dest;

    while (size--)
        *p++ = (unsigned char) ch;
}

void* ba_memcpy(void *dest, const void *src, size_t size)
{
    uint8_t *d = (uint8_t *) dest;
    const uint8_t *s = (const uint8_t *) src;

    while (size--)
        *d++ = *s++;

    return dest;
}

const char *ba_strrchr(const char *str, int ch)
{
    const char *p = str;

    /* Find null character */
    while (*p != '\0')
        ++p;

    while (p != str) {
        if (*p == ch)
            return p;
        --p;
    }

    return 0;
}

const char *ba_strcpy(char *dest, const char *src)
{
    char *d = dest;
    const char *s = src;

    while ('\0' != (*d++ = *s++));

    return dest;
}

/** ***************** stdio **************************/

void ba_printf(const char* fmt, ... )
{
    va_list arglist;

    va_start(arglist, fmt);
    vprintf(fmt, arglist);

    va_end(arglist);
}

/** ***************** stdlib **************************/

void* ba_malloc(uint32_t size)
{
    void *ptr = malloc(size);

    if (ptr) {
        ba_memset(ptr, 0x00, size);
    }

    return ptr;
}

void ba_free(void *addr)
{
    free(addr);
}

void ba_register_on_exit_callback(benchapp_on_exit_t on_exit_cback)
{
    atexit(on_exit_cback);
}

int ba_rand(void)
{
    return rand();
}

/** ***************** time **************************/

int ba_clock_gettime(test_timespec_t *ts)
{
    return clock_gettime(CLOCK_MONOTONIC, (struct timespec *) ts);
}

float ba_time_diff(void *arg_start, void *arg_end, int diff_type)
{
    BA_STATIC_ASSERT(sizeof(struct timespec) <= sizeof(test_timespec_t));

    struct timespec *start = (struct timespec *) arg_start;
    struct timespec *end   = (struct timespec *) arg_end;

    float result = ((end->tv_sec - start->tv_sec) * 1000 * 1000 * 1000) + end->tv_nsec - start->tv_nsec;

    switch (diff_type) {
    case DIFF_SEC:
        result /= 1000;
    case DIFF_MSEC:
        result /= 1000;
    case DIFF_USEC:

        result /= 1000;
    default:
        break;
    }

    return result;
}

int ba_usleep(uint32_t usec)
{
    return usleep(usec);
}
