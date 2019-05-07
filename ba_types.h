#ifndef BATYPES_H_INCLUDED
#define BATYPES_H_INCLUDED

#define VARIATION_MAX                   16

#define DIFF_USEC			            0
#define DIFF_MSEC			            1
#define DIFF_SEC			            2

#define BA_STATIC_ASSERT(cond)   (sizeof(char[1 - 2*!(cond)]))

typedef void  (*benchapp_on_exit_t) (void);

typedef struct {
    uint64_t sec;
    uint64_t nsec;
} test_timespec_t;

typedef void* (*benchapp_init_t)    (void);
typedef void  (*benchapp_uninit_t)  (void *);
typedef int   (*benchapp_run_t)     (void *);

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

  test_timespec_t ts_start;
  test_timespec_t ts_end;

  unsigned long cycle;
  unsigned long duration;

  benchapp_run_t   runner;

  uint8_t nb_variations;
  uint8_t variations[VARIATION_MAX];
  uint8_t variation_names[VARIATION_MAX][32];
  int curr_variation_index;

  int current_recipe_index;
  int max_recipe;
  test_recipe_t *recipes;
} test_info_common_t;

#endif /* BATYPES_H_INCLUDED */
