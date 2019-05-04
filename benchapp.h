#ifndef BENCHAPP_H_INCLUDED
#define BENCHAPP_H_INCLUDED

#include <stdint.h>
#include <stddef.h>
#include <time.h>

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

#define DIFF_SEC			            2
#define DIFF_MSEC			            1
#define DIFF_USEC			            0

#ifndef RELEASE_MODE
#define benchapp_logf(...) do { \
                            const char *f = __FILE__; \
                            const char *p = strrchr(f, '\/'); \
                            int index = (p) ? (p - f + 1) : 0; \
                            benchapp_printf("[ %s (%d) ] ", f + index, __LINE__); \
                            benchapp_printf(__VA_ARGS__);  \
                            benchapp_printf("\n"); \
                           } while (0)
#else
#define benchapp_logf(...)
#endif

typedef void* (*benchapp_init_t) (void);
typedef int   (*benchapp_run_t)  (void *arg);

typedef struct {
  char name[255];

  int state;
  int status;

  uint32_t cycle;
  unsigned long duration;

  struct timespec ts_start;
  struct timespec ts_end;

  benchapp_run_t   runner;
} test_info_common_t;

void benchapp_printf(const char* fmt, ... );
void* benchapp_malloc(size_t size);

#define INCLUDE_TEST(test)     extern void* benchapp_init_##test  (void)

#endif /* BENCHAPP_H_INCLUDED */
