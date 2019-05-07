#include "benchapp.h"
#include "WjCryptLib_Aes.h"
#include "WjCryptLib_AesCbc.h"
#include "WjCryptLib_AesCtr.h"
#include "WjCryptLib_AesOfb.h"

#define AES_KEYSIZE_TYPE_COUNT  3

typedef struct {
  test_info_common_t c;
  
  AesContext ctx[AES_KEYSIZE_TYPE_COUNT]; /* 128 | 192 | 256 */
  AesCbcContext cbc_ctx[AES_KEYSIZE_TYPE_COUNT]; /* 128 | 192 | 256 */

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

      for (int j = 0; j < max_key_length; ++j) {
        info->key[j] = ba_rand() % 256;

        switch (j) {
        case AES_KEY_SIZE_128:
            AesInitialise(&info->ctx[0], info->key, AES_KEY_SIZE_128);
            AesCbcInitialise(&info->cbc_ctx[0], &info->ctx[0], info->key);

            break;
        case AES_KEY_SIZE_192:
            AesInitialise(&info->ctx[1], info->key, AES_KEY_SIZE_192);
            AesCbcInitialise(&info->cbc_ctx[1], &info->ctx[1], info->key);
            break;
        }
      }

      for (int j = 0; j < AES_CBC_IV_SIZE; ++j) {
        info->iv[j] = ba_rand() % 256;
      }

      AesInitialise(&info->ctx[2], info->key, AES_KEY_SIZE_256);
      AesCbcInitialise(&info->cbc_ctx[2], &info->ctx[2], info->iv);

      info->c.status = TEST_STATUS_INITIALIZED;
    }
    break;

  case TEST_STATE_RUN:
    {
#if 0
      for (int i = 0; i < info->data_size; ++i)
          info->data[i] = ba_rand() % 256;
#else
      memcpy(info->data, info->encrypted, info->data_size);
#endif

      AesCbcEncrypt(&info->cbc_ctx[1], info->data, info->encrypted, info->data_size);
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

        info->c.recipes = info->recipes;
        info->c.current_recipe_index = 0;
        info->c.max_recipe = 0;

        ba_memset(info->recipes, 0x00, sizeof(info->recipes));

        ba_strcpy(info->recipes[info->c.max_recipe].name, "AES");
        info->recipes[info->c.max_recipe].type = RTYPE_TIME_LIMITED;
        info->recipes[info->c.max_recipe].duration = 5000 * 1000;
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
