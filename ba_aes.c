#include "benchapp.h"
#include "WjCryptLib_Aes.h"
#include "WjCryptLib_AesCbc.h"
#include "WjCryptLib_AesCtr.h"
#include "WjCryptLib_AesOfb.h"

#define AES_KEYSIZE_TYPE_COUNT  3

typedef struct {
  test_info_common_t c;
  
  AesContext ctx; /* 128 | 192 | 256 */
  AesCbcContext cbc_ctx; /* 128 | 192 | 256 */

  uint8_t key[AES_KEY_SIZE_256];
  uint8_t iv[AES_CBC_IV_SIZE];

  uint8_t *data;
  uint8_t *encrypted;
  uint8_t *decrypted;
  uint32_t data_size;

  test_recipe_t recipes[5];
} aes_test_info_t;

int benchapp_run_AES(void *arg)
{
  aes_test_info_t *info = (aes_test_info_t *) arg;
  
  switch (info->c.state) {
  case TEST_STATE_INIT:
    {
      int max_key_length = AES_KEY_SIZE_256;
      int key_size;

      for (int j = 0; j < max_key_length; ++j) {
        info->key[j] = ba_rand() % 256;
      }

      for (int j = 0; j < AES_CBC_IV_SIZE; ++j) {
        info->iv[j] = ba_rand() % 256;
      }

      switch (info->c.curr_variation_index) {
      case 0:
          key_size = AES_KEY_SIZE_128;
          benchapp_logf("Variation Key Size: 128 Bits");
          break;

      case 1:
          key_size = AES_KEY_SIZE_192;
          benchapp_logf("Variation Key Size: 192 Bits");
          break;

      case 2:
          key_size = AES_KEY_SIZE_256;
          benchapp_logf("Variation Key Size: 256 Bits");
          break;
      }

      AesInitialise(&info->ctx, info->key, key_size);
      AesCbcInitialise(&info->cbc_ctx, &info->ctx, info->key);

      info->c.status = TEST_STATUS_INITIALIZED;
    }
    break;

  case TEST_STATE_RUN:
    {
#if 0
      for (int i = 0; i < info->data_size; ++i)
          info->data[i] = ba_rand() % 256;
#else
      ba_memcpy(info->data, info->encrypted, info->data_size);
#endif

      AesCbcEncrypt(&info->cbc_ctx, info->data, info->encrypted, info->data_size);
      info->c.status = TEST_STATUS_RUNNING;
    }
    break;

  case TEST_STATE_UNINIT:
    {
        info->c.status = TEST_STATUS_UNINITIALIZED;

        double throughput = (info->c.cycle * info->data_size) / (1024. * 1024.);
        double duration_in_secs = (info->c.duration / 1000000.);

        ba_printf("Total Throughput: %1.3lf MB\n", throughput);
        ba_printf("Speed: %1.3lf MB/s\n", (throughput / duration_in_secs));
    }
    break;

  }

  return BENCH_TEST_SUCCESS;
}

void* benchapp_init_AES(void)
{
    aes_test_info_t *info = (aes_test_info_t *) ba_malloc(sizeof(aes_test_info_t));

    if (info) {
        ba_memset(info, 0x00, sizeof(aes_test_info_t));

        ba_strcpy(info->c.name, "AES");
        info->c.runner = &benchapp_run_AES;

        info->data_size  = (1024 * 1024);
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
        info->c.nb_variations = 3;
        info->c.variations[0] = 1;
        ba_strcpy(info->c.variation_names[0], "128 Bit");
        info->c.variations[1] = 1;
        ba_strcpy(info->c.variation_names[1], "192 Bit");
        info->c.variations[2] = 1;
        ba_strcpy(info->c.variation_names[2], "256 Bit");

        ba_memset(info->recipes, 0x00, sizeof(info->recipes));
        info->c.recipes = info->recipes;
        info->c.current_recipe_index = 0;
        info->c.max_recipe = 0;

        ba_strcpy(info->recipes[info->c.max_recipe].name, "AES Time Limited");
        info->recipes[info->c.max_recipe].type = RTYPE_TIME_LIMITED;
        info->recipes[info->c.max_recipe].duration = 5000 * 1000;
        ++info->c.max_recipe;
        ba_strcpy(info->recipes[info->c.max_recipe].name, "AES Execution Limited");
        info->recipes[info->c.max_recipe].type = RTYPE_EXECUTION_LIMITED;
        info->recipes[info->c.max_recipe].max_cycle = 1000;
        ++info->c.max_recipe;
    }

    return info;
}

void benchapp_uninit_AES(void *arg)
{
    aes_test_info_t *info = (aes_test_info_t *) arg;

    if (info) {
        ba_free(info->data);
        ba_free(info);
    }
}
