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

      info->c.data_size  = 1024 * 1024;
      info->c.data = calloc(1, info->c.data_size);
      for (int i = 0; i < info->c.data_size; ++i) 
        info->c.data[i] = rand() % 256;
      info->curr_block_index = 0;
      info->block_size = 4096;
      info->c.status = TEST_STATUS_INITIALIZED;
    }
    break;

  case TEST_STATE_RUN: 
    if (info->curr_block_index  * info->block_size >= info->c.data_size) {
      /* printf("Index reset!\n"); */ 
      info->curr_block_index = 0;
    }

    Md5Update(&info->ctx, info->c.data + info->curr_block_index * info->block_size, info->block_size);
    info->c.status = TEST_STATUS_RUNNING;
    ++info->curr_block_index;
    break;

  case TEST_STATE_UNINIT:
    Md5Finalise(&info->ctx, &info->digest);
    free(info->c.data);
    info->c.data = 0;
    info->c.data_size = 0;
    info->c.status = TEST_STATUS_UNINITIALIZED;
    break;

  }

  return BENCH_TEST_SUCCESS;
}
