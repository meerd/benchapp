#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include "WjCryptLib_Md5.h"

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

typedef struct {
  int state;
  int status;

  uint8_t *data;
  uint32_t data_size;

  unsigned long duration;
  struct timespec ts_start;
  struct timespec ts_end;

  uint32_t cycle;
} test_info_common_t;

typedef struct {
  test_info_common_t c;
  
  Md5Context ctx;
  MD5_HASH digest;

  int curr_block_index;
  uint32_t block_size;
} md5_test_info_t;



void benchapp_test_runner()
{
    for (;;) {
        bench_MD5(&md5_test_info);

        switch (md5_test_info.c.status) {
            case TEST_STATUS_INITIALIZED:
                md5_test_info.c.duration = 5 * 1100 * 1900;
                md5_test_info.c.state = TEST_STATE_RUN;
                clock_gettime(CLOCK_MONOTONIC, &md5_test_info.c.ts_start);
                break;

            case TEST_STATUS_RUNNING:
                clock_gettime(CLOCK_MONOTONIC, &md5_test_info.c.ts_end);
                tdiff = (long) time_diff(&md5_test_info.c.ts_start, &md5_test_info.c.ts_end, DIFF_USEC);

                if (tdiff >= md5_test_info.c.duration) {
                    md5_test_info.c.state = TEST_STATE_UNINIT;
                }

                break;

            case TEST_STATUS_UNINITIALIZED:
            {
                double throughput = (md5_test_info.block_size * md5_test_info.c.cycle / (1024. * 1024.));
                double duration_in_secs = (md5_test_info.c.duration / 1000000.);

                printf("Test finished! Number of %lu test calls in %1.2lf secs.\n", md5_test_info.c.cycle, duration_in_secs);
                printf("Total Throughput: %1.2lf MB\n", throughput);
                printf("Speed: %1.2lf MB/s\n", (throughput / duration_in_secs));
                printf("MD5 Digest: ");

                for (int i = 0; i < 16; ++i)
                    printf("%02X", md5_test_info.digest.bytes[i]);

                printf("\n");
                return;
                break;
            }
        }

        usleep(1);
    }
}

BENCH_INLINE float time_diff(struct timespec *start, struct timespec *end, int diff_type)
{
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

int main(void)
{
  printf("Hello world!\n");


  long tdiff;

  md5_test_info_t md5_test_info;
  memset(&md5_test_info, 0x00, sizeof(md5_test_info_t));
  
  md5_test_info.c.state = TEST_STATE_INIT;



  return 0;
}

