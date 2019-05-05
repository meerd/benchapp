#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include "WjCryptLib_Md5.h"

#include "benchapp.h"

INCLUDE_TEST(MD5);

benchapp_init_t init_calls[] = {
    &benchapp_init_MD5
};

static int failures = 0;

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

void benchapp_test_runner(test_info_common_t *test_info)
{
    if (test_info) {
        test_info->state = TEST_STATE_INIT;
        for (;;) {
            if (BENCH_TEST_SUCCESS != test_info->runner(test_info)) {
                benchapp_printf("Test (%s) has failed at cycle: %d\n", test_info->name, test_info->cycle);
                ++failures;
                return;
            }

            switch (test_info->status) {
                case TEST_STATUS_INITIALIZED:
                    test_info->state = TEST_STATE_RUN;
                    clock_gettime(CLOCK_MONOTONIC, &test_info->ts_start);
                    test_info->cycle = 0;
                    test_info->duration = 0;
                    break;

                case TEST_STATUS_RUNNING:
                    {
                        test_recipe_t *current_recipe =&test_info->recipes[test_info->current_recipe_index];
                        clock_gettime(CLOCK_MONOTONIC, &test_info->ts_end);
                        long tdiff = (long) time_diff(&test_info->ts_start, &test_info->ts_end, DIFF_USEC);

                        test_info->duration += tdiff;
                        if (RTYPE_TIME_LIMITED == current_recipe->type) {
                            if (test_info->duration >= current_recipe->duration) {
                                test_info->state = TEST_STATE_UNINIT;
                            }
                        } else { /* RTYPE_EXECUTION_LIMITED */
                            current_recipe->duration = tdiff;
                            if (test_info->cycle >= current_recipe->max_cycle) {
                                test_info->state = TEST_STATE_UNINIT;
                            }
                        }

                    }

                   if (TEST_STATE_UNINIT != test_info->state) {
                       clock_gettime(CLOCK_MONOTONIC, &test_info->ts_start);
                       ++test_info->cycle;
                   }
                   break;

                case TEST_STATUS_UNINITIALIZED:
                    {
                        test_recipe_t *current_recipe =&test_info->recipes[test_info->current_recipe_index];
                        double duration_in_secs = (test_info->duration / 1000000.);

                        benchapp_printf("Test finished! Number of %lu test calls in %1.2lf secs.\n", test_info->cycle, duration_in_secs);
                        test_info->state = TEST_STATE_IDLE;
                        return;
                    }
                    break;
            }

            usleep(1);
        }
    }
}

void app_exit(void)
{
    if (0 == failures)
        benchapp_printf("Application has exited successfully.\n");
    else
        benchapp_printf("Application has exited with %d failures!\n", failures);
}

int main(void)
{
  benchapp_logf("Application has started!");

  atexit(app_exit);

  for (int i = 0; i < sizeof(init_calls) / sizeof(init_calls[0]); ++i) {
      if (init_calls[i]) {
          test_info_common_t *info = init_calls[i]();
          benchapp_logf("Running test: %s | Number of recipes: %d", info->name, info->max_recipe);
          for (int r = 0; r < info->max_recipe; ++r) {
              if (RTYPE_NONE != info->recipes[i].type) {
                  info->current_recipe_index = r;
                  benchapp_logf("%d) Running recipe: %s", r + 1, info->recipes[info->current_recipe_index].name);
                  benchapp_test_runner(info);
              }
          }
      }
  }

  return 0;
}
