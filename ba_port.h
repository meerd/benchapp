#ifndef BA_PORT_H_INCLUDED
#define BA_PORT_H_INCLUDED

#include "ba_types.h"

void  ba_printf(const char* fmt, ... );
void* ba_malloc(uint32_t size);
void  ba_free(void *addr);
void  ba_memset(void *dest, int ch, size_t size);
void* ba_memcpy(void *dest, const void *src, size_t size);

const char *ba_strrchr(const char *str, int ch);
const char *ba_strcpy(char *dest, const char *src);

void ba_register_on_exit_callback(benchapp_on_exit_t on_exit_cback);

int ba_clock_gettime(test_timespec_t *ts);
float ba_time_diff(void *start, void *end, int diff_type);
int ba_usleep(uint32_t usec);

int ba_rand(void);

#endif /* BA_PORT_H_INCLUDED */
