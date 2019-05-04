#ifndef BENCHAPP_H_INCLUDED
#define BENCHAPP_H_INCLUDED

#include <stdint.h>
#include <time.h>

#define BENCH_TEST_SUCCESS              0

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

typedef void* (*benchapp_init_t) (void);
typedef int   (*benchapp_run_t)  (void *arg);

typedef struct {
  int state;
  int status;

  uint8_t *data;
  uint32_t data_size;

  uint32_t cycle;
  unsigned long duration;

  struct timespec ts_start;
  struct timespec ts_end;

  benchapp_init_t) init;
  benchapp_run_t   run;
} test_info_common_t;

#endif /* BENCHAPP_H_INCLUDED */
