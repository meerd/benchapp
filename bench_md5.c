#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include "WjCryptLib_Md5.h"

#include "benchapp.h"

typedef struct {
  test_info_common_t c;
  
  Md5Context ctx;
  MD5_HASH digest;

  uint8_t *data;
  uint32_t data_size;

  int curr_block_index;
  uint32_t block_size;
} md5_test_info_t;

int benchapp_run_MD5(void *arg)
{
  md5_test_info_t *info = (md5_test_info_t *) arg;
  ++info->c.cycle;
  
  switch (info->c.state) {
  case TEST_STATE_INIT:
    { 
      Md5Initialise(&info->ctx);
      info->c.status = TEST_STATUS_INITIALIZED;
    }
    break;

  case TEST_STATE_RUN: 
    if (info->curr_block_index  * info->block_size >= info->data_size) {
      /* printf("Index reset!\n"); */ 
      info->curr_block_index = 0;
    }

    Md5Update(&info->ctx, info->data + info->curr_block_index * info->block_size, info->block_size);
    info->c.status = TEST_STATUS_RUNNING;
    ++info->curr_block_index;
    break;

  case TEST_STATE_UNINIT:
    {
        Md5Finalise(&info->ctx, &info->digest);
        free(info->data);
        info->data = 0;
        info->data_size = 0;
        info->c.status = TEST_STATUS_UNINITIALIZED;

        benchapp_printf("MD5 Digest: ");
        for (int i = 0; i < 16; ++i)
            benchapp_printf("%02X", info->digest.bytes[i]);
        benchapp_printf("\n");

        double throughput = (info->block_size * info->c.cycle / (1024. * 1024.));
        double duration_in_secs = (info->c.duration / 1000000.);

        benchapp_printf("Total Throughput: %1.2lf MB\n", throughput);
        benchapp_printf("Speed: %1.2lf MB/s\n", (throughput / duration_in_secs));

    }
    break;

  }

  return BENCH_TEST_SUCCESS;
}

void* benchapp_init_MD5(void)
{
    md5_test_info_t *info = (md5_test_info_t *) benchapp_malloc(sizeof(md5_test_info_t));

    if (info) {
        strcpy(info->c.name, "MD5");

        info->c.runner = &benchapp_run_MD5;
        info->c.duration = 5 * 1000 * 1000;

        info->curr_block_index = 0;
        info->block_size = 4096;

        info->data_size  = (1024 * 1024);
        info->data = benchapp_malloc(info->data_size);

        for (int i = 0; i < info->data_size; ++i)
          info->data[i] = rand() % 256;
    }

    return info;
}
