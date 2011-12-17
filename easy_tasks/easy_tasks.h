#define DESC_LEN 1001
#define ID_LEN 7
#define CATEGORY_LEN 101
#define DATA_FORMAT "%d %m %Y"

struct category {
  char name[CATEGORY_LEN];
};
struct task {
  char id[ID_LEN];
  char category[CATEGORY_LEN];
  time_t end_date;
  int nedded_time;
  char desc[DESC_LEN];
};

