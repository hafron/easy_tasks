#define DESC_LEN 1000
#define ID_LEN 7
#define CATEGORY_LEN 100
#define DATA_FORMAT "%d %m %Y"


struct task {
  char id[ID_LEN];
  char category[ID_LEN];
  time_t end_date;
  int nedded_time;
  char desc[DESC_LEN];
};

struct category {
  char id[ID_LEN];
  char name[CATEGORY_LEN];
};