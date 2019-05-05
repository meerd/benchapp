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
                            const char *p = strrchr(f, '/'); \
                            int index = (p) ? (p - f + 1) : 0; \
                            benchapp_printf("[ %s (%d) ] ", f + index, __LINE__); \
                            benchapp_printf(__VA_ARGS__);  \
                            benchapp_printf("\n"); \
                           } while (0)
#else
#define benchapp_logf(...)
#endif

#define INCLUDE_TEST(test)     extern void* benchapp_init_##test  (void)

typedef void* (*benchapp_init_t) (void);
typedef int   (*benchapp_run_t)  (void *arg);

typedef enum {
    RTYPE_NONE,
    RTYPE_TIME_LIMITED,
    RTYPE_EXECUTION_LIMITED
} test_recipe_type_t;

typedef struct {
    test_recipe_type_t type;
    char name[32];

    unsigned long duration;
    uint32_t max_cycle;
} test_recipe_t;

typedef struct {
  char name[255];

  int state;
  int status;

  struct timespec ts_start;
  struct timespec ts_end;

  unsigned long cycle;
  unsigned long duration;

  benchapp_run_t   runner;

  int current_recipe_index;
  int max_recipe;
  test_recipe_t *recipes;
} test_info_common_t;

void benchapp_printf(const char* fmt, ... );
void* benchapp_malloc(size_t size);

BENCH_INLINE float time_diff(struct timespec *start, struct timespec *end, int diff_type);

#endif /* BENCHAPP_H_INCLUDED */
