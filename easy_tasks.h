#define DESC_LEN 1000
#define ID_LEN 7
#define CATEGORY_LEN 100
#define DATA_FORMAT "%F"

#define MAX_ERRORS 7
#define MAX_ERROR_LEN 100

struct task {
  char id[ID_LEN];
  char category[ID_LEN];
  time_t date;
  int time_needed;
  char desc[DESC_LEN];
};

struct category {
  char id[ID_LEN];
  char name[CATEGORY_LEN];
};