#include "benchapp.h"

INCLUDE_TEST(MD5);
INCLUDE_TEST(AES);

benchapp_init_t init_calls[] = {
    &benchapp_init_MD5,
    &benchapp_init_AES
};

benchapp_uninit_t uninit_calls[] = {
    &benchapp_uninit_MD5,
    &benchapp_init_AES
};

static int failures = 0;

static void benchapp_test_runner(test_info_common_t *test_info)
{
    if (test_info) {
        test_info->state = TEST_STATE_INIT;
        for (;;) {
            if (BENCH_TEST_SUCCESS != test_info->runner(test_info)) {
                ba_printf("Test (%s) has failed at cycle: %d\n", test_info->name, test_info->cycle);
                ++failures;
                return;
            }

            switch (test_info->status) {
                case TEST_STATUS_INITIALIZED:
                    test_info->state = TEST_STATE_RUN;
                    ba_clock_gettime(&test_info->ts_start);
                    test_info->cycle = 0;
                    test_info->duration = 0;
                    break;

                case TEST_STATUS_RUNNING:
                    {
                        test_recipe_t *current_recipe = &test_info->recipes[test_info->current_recipe_index];

                        ba_clock_gettime(&test_info->ts_end);
                        long tdiff = (long) ba_time_diff(&test_info->ts_start, &test_info->ts_end, DIFF_USEC);

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
                       ba_clock_gettime(&test_info->ts_start);
                       ++test_info->cycle;
                   }
                   break;

                case TEST_STATUS_UNINITIALIZED:
                    {
                        test_recipe_t *current_recipe = &test_info->recipes[test_info->current_recipe_index];
                        double duration_in_secs = (test_info->duration / 1000000.);

                        ba_printf("Finished: Number of %lu test calls in %1.2lf secs.\n", test_info->cycle, duration_in_secs);
                        test_info->state = TEST_STATE_IDLE;
                        return;
                    }
                    break;
            }

            ba_usleep(0);
        }
    }
}

static void app_exit(void)
{
    if (0 == failures)
        ba_printf("The application has exited successfully.\n");
    else
        ba_printf("The application has exited with %d failures!\n", failures);

    ba_printf("|*************************************|\n");
}

static void run_recipes(test_info_common_t *info)
{
    for (int r = 0; r < info->max_recipe; ++r) {
        if (RTYPE_NONE != info->recipes[r].type) {
            info->current_recipe_index = r;
            ba_printf("Running recipe: %s\n", info->recipes[info->current_recipe_index].name);
            benchapp_test_runner(info);
            ba_printf("--------------------------------------\n");
        }
    }
}

int main(void)
{
  ba_printf("|*************************************|\n");
  ba_printf("          Bencapp has started!\n");
  ba_register_on_exit_callback(app_exit);

  for (int i = 0; i < sizeof(init_calls) / sizeof(init_calls[0]); ++i) {
      if (init_calls[i]) {
          test_info_common_t *info = init_calls[i]();
          ba_printf("######################################\n");
          if (info->nb_variations) {
              /* Execute Variations */
              for (int v = 0; v < VARIATION_MAX; ++v) {
                  if (info->variations[v]) {
                      info->curr_variation_index = v;
                      ba_printf("Test: %s / Variation: %s / Number of Recipes: %d\n", info->name, info->variation_names[v], info->max_recipe);
                      /* Recipes */
                      run_recipes(info);
                  }
              }
          } else {
              /* No variation exists. */
              ba_printf("Test: %s / Number of Recipes: %d\n", info->name, info->max_recipe);
              /* Recipes */
              run_recipes(info);
          }
          ba_printf("######################################\n");
          uninit_calls[i](info);
      }
  }

  return 0;
}
