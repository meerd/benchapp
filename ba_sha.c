#include "benchapp.h"
#include "WjCryptLib_Sha1.h"
#include "WjCryptLib_Sha256.h"
#include "WjCryptLib_Sha512.h"

typedef struct {
  test_info_common_t c;
  
  Sha1Context ctx_160;
  Sha256Context ctx_256;
  Sha512Context ctx_512;

  uint8_t *data;
  uint8_t *encrypted;
  uint8_t *decrypted;
  uint32_t data_size;

  int curr_block_index;
  uint32_t block_size;

  test_recipe_t recipes[5];

  uint8_t digest_output[SHA512_HASH_SIZE];
  uint32_t digest_length;
} sha_test_info_t;

int benchapp_run_SHA(void *arg)
{
  sha_test_info_t *info = (sha_test_info_t *) arg;
  
  switch (info->c.state) {
  case TEST_STATE_INIT:
    {
      switch (info->c.curr_variation_index) {
      case 0:
          Sha1Initialise(&info->ctx_160);
          benchapp_logf("Variation: 160 Bits");
          break;

      case 1:
          Sha256Initialise(&info->ctx_256);
          benchapp_logf("Variation Key Size: 256 Bits");
          break;

      case 2:
          Sha512Initialise(&info->ctx_512);
          benchapp_logf("Variation Key Size: 512 Bits");
          break;
      }
      info->c.status = TEST_STATUS_INITIALIZED;
    }
    break;

  case TEST_STATE_RUN:
    {
      info->c.status = TEST_STATUS_RUNNING;

      if (info->curr_block_index  * info->block_size >= info->data_size) {
        /* printf("Index reset!\n"); */
        info->curr_block_index = 0;
      }

      switch (info->c.curr_variation_index) {
      case 0:
          Sha1Update(&info->ctx_160, info->data + info->curr_block_index * info->block_size, info->block_size);
          break;

      case 1:
          Sha256Update(&info->ctx_256, info->data + info->curr_block_index * info->block_size, info->block_size);
          break;

      case 2:
          Sha512Update(&info->ctx_512, info->data + info->curr_block_index * info->block_size, info->block_size);
          break;
      }

      ++info->curr_block_index;
      info->c.status = TEST_STATUS_RUNNING;
    }
    break;

  case TEST_STATE_UNINIT:
    {
        info->c.status = TEST_STATUS_UNINITIALIZED;

        switch (info->c.curr_variation_index) {
        case 0:
            Sha1Finalise(&info->ctx_160, (SHA1_HASH *) &info->digest_output);
            info->digest_length = SHA1_HASH_SIZE;
            break;

        case 1:
            Sha256Finalise(&info->ctx_256, (SHA256_HASH *) &info->digest_output);
            info->digest_length = SHA256_HASH_SIZE;
            break;

        case 2:
            Sha512Finalise(&info->ctx_512, (SHA512_HASH *) &info->digest_output);
            info->digest_length = SHA512_HASH_SIZE;
            break;
        }

        ba_printf("SHA Digest: ");
        for (int i = 0; i < info->digest_length; ++i)
            ba_printf("%02X", info->digest_output[i]);
        ba_printf("\n");

        double throughput = (info->c.cycle * info->block_size) / (1024. * 1024.);
        double duration_in_secs = (info->c.duration / 1000000.);

        ba_printf("Total Throughput: %1.3lf MB\n", throughput);
        ba_printf("Speed: %1.3lf MB/s\n", (throughput / duration_in_secs));
    }
    break;

  }

  return BENCH_TEST_SUCCESS;
}

void* benchapp_init_SHA(void)
{
    sha_test_info_t *info = (sha_test_info_t *) ba_malloc(sizeof(sha_test_info_t));

    if (info) {
        ba_memset(info, 0x00, sizeof(sha_test_info_t));

        ba_strcpy(info->c.name, "SHA");
        info->c.runner = &benchapp_run_SHA;

        info->curr_block_index = 0;
        info->block_size = CONFIG_BENCHMARK_SHA_TEST_BLOCK_SIZE;

        info->data_size  = CONFIG_BENCHMARK_SHA_TEST_DATA_SIZE;
        info->data = ba_malloc(info->data_size);
        info->encrypted = ba_malloc(info->data_size);
        info->decrypted = ba_malloc(info->data_size);

        for (int i = 0; i < info->data_size; ++i)
          info->data[i] = ba_rand() % 256;

        ba_memset(info->encrypted, 0x00, info->data_size);
        ba_memset(info->decrypted, 0x00, info->data_size);

        ba_memset(info->c.variations, 0x00, sizeof(info->c.variations));
        ba_memset(info->c.variation_names, 0x00, sizeof(info->c.variation_names));

        info->c.curr_variation_index = 0;
        info->c.nb_variations = 0;

#ifdef CONFIG_BENCHMARK_SHA_VARIATION_160B
        info->c.variations[0] = 1;
        ba_strcpy(info->c.variation_names[0], "160 Bit");
        ++info->c.nb_variations;
#endif

#ifdef CONFIG_BENCHMARK_SHA_VARIATION_256B
        info->c.variations[1] = 1;
        ba_strcpy(info->c.variation_names[1], "256 Bit");
        ++info->c.nb_variations;
#endif

#ifdef CONFIG_BENCHMARK_SHA_VARIATION_512B
        info->c.variations[2] = 1;
        ba_strcpy(info->c.variation_names[2], "512 Bit");
        ++info->c.nb_variations;
#endif

        ba_memset(info->recipes, 0x00, sizeof(info->recipes));
        info->c.recipes = info->recipes;
        info->c.current_recipe_index = 0;
        info->c.max_recipe = 0;

#ifdef CONFIG_BENCHMARK_SHA_RECIPE1
#ifdef CONFIG_BENCHMARK_SHA_R1_TIME_LIMITED
        ba_strcpy(info->recipes[info->c.max_recipe].name, CSTR(CONFIG_BENCHMARK_SHA_R1_NAME));
        info->recipes[info->c.max_recipe].type = RTYPE_TIME_LIMITED;
        info->recipes[info->c.max_recipe].duration = CONFIG_BENCHMARK_SHA_RECIPE1_TIME * 1000;
#else
        ba_strcpy(info->recipes[info->c.max_recipe].name, CSTR(CONFIG_BENCHMARK_SHA_R1_NAME));
        info->recipes[info->c.max_recipe].type = RTYPE_EXECUTION_LIMITED;
        info->recipes[info->c.max_recipe].max_cycle = CONFIG_BENCHMARK_SHA_RECIPE1_CALLS;
#endif
        ++info->c.max_recipe;
#endif

#ifdef CONFIG_BENCHMARK_SHA_RECIPE2
#ifdef CONFIG_BENCHMARK_SHA_R2_TIME_LIMITED
        ba_strcpy(info->recipes[info->c.max_recipe].name, CSTR(CONFIG_BENCHMARK_SHA_R2_NAME));
        info->recipes[info->c.max_recipe].type = RTYPE_TIME_LIMITED;
        info->recipes[info->c.max_recipe].duration = CONFIG_BENCHMARK_SHA_RECIPE2_TIME * 1000;
#else
        ba_strcpy(info->recipes[info->c.max_recipe].name, CSTR(CONFIG_BENCHMARK_SHA_R2_NAME));
        info->recipes[info->c.max_recipe].type = RTYPE_EXECUTION_LIMITED;
        info->recipes[info->c.max_recipe].max_cycle = CONFIG_BENCHMARK_SHA_RECIPE2_CALLS;
#endif
        ++info->c.max_recipe;
#endif
    }

    return info;
}

void benchapp_uninit_SHA(void *arg)
{
    sha_test_info_t *info = (sha_test_info_t *) arg;

    if (info) {
        ba_free(info->data);
        ba_free(info);
    }
}
