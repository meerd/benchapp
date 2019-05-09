#ifndef BENCHAPP_H_INCLUDED
#define BENCHAPP_H_INCLUDED

#define BENCH_TEST_SUCCESS              0
#define BENCH_TEST_FAILURE              1

#define BENCH_INLINE              

#define TEST_STATE_IDLE                 0
#define TEST_STATE_INIT                 1
#define TEST_STATE_RUN                  2
#define TEST_STATE_UNINIT               3

#define TEST_STATUS_NONE                0
#define TEST_STATUS_INITIALIZED         1
#define TEST_STATUS_RUNNING             2
#define TEST_STATUS_UNINITIALIZED       3

#define CSTR(a) BCSTR(a)
#define BCSTR(a) #a

/* DEBUG_MESSAGES */

#ifdef DEBUG_MESSAGES
#define benchapp_logf(...) do { \
                            const char *f = __FILE__; \
                            const char *p = ba_strrchr(f, '/'); \
                            int index = (p) ? (p - f + 1) : 0; \
                            ba_printf("[ %s (%d) ] ", f + index, __LINE__); \
                            ba_printf(__VA_ARGS__);  \
                            ba_printf("\n"); \
                           } while (0)
#else
#define benchapp_logf(...)
#endif

#define INCLUDE_TEST(test)     extern void* benchapp_init_##test   (void);  \
                               extern void  benchapp_uninit_##test (void *)

#include "ba_std.h"
#include "ba_port.h"

#endif /* BENCHAPP_H_INCLUDED */
