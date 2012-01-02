#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <regex.h>

#include "easy_tasks.h" 
#include "driver.h"

/**
 * OUTPUT_FORMAT defines format of output. Current avalible values are "STDOUT" - for text mode usage and JSON - if you want to get output in JSON format.
 */
char OUTPUT_FORMAT[MAX_FORMAT_LEN] = "STDOUT";
char * ERRORS[MAX_ERRORS];
int ERR_NO = 0;
char * AVAILABLE_FORMATS[MAX_FORMATS] = {"STDOUT", "JSON", "", "", ""};

void throw_errors();
void add_category(struct category *);
void add_task(struct task *);
void show_categories();
void show_tasks(int, char *, int);
void error(char *);
void get_category_values(int, char **, struct category *);
void get_task_values(int, char **, struct task *);

char * upper(char *);
int in_array(char *, char **, int);
void set_output_format(char *);

/**
 * Function reads console params and calls appropriate functions.
 * 
 */
int main(int agrc, char * agrv[])
{
  int i, j, limit=-1, desc=0;
  char sort_by[COL_TITLE_LEN] = "date";
  struct category category_to_add;
  struct task task_to_add;
  
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-f") == 0)
    {
      if(i+1 < agrc)
	set_output_format(agrv[i+1]);
    }
  }
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-a") == 0)
    {
      for(j=1;j<agrc;j++)
      {
        if(strcmp(agrv[j], "-c") == 0)
        {
          get_category_values(agrc, agrv, &category_to_add);
          throw_errors();
          add_category(&category_to_add);
          throw_errors();
          return 0;
        } else if(strcmp(agrv[j], "-t") == 0)
        {
          get_task_values(agrc, agrv, &task_to_add);
          throw_errors();
          add_task(&task_to_add);
          throw_errors();
          return 0;
        }
      }
    } else if(strcmp(agrv[i], "-ac") == 0 || strcmp(agrv[i], "-ca") == 0) {
      get_category_values(agrc, agrv, &category_to_add);
      throw_errors();
      add_category(&category_to_add);
      throw_errors();
      return 0;
    } else if(strcmp(agrv[i], "-at") == 0 || strcmp(agrv[i], "-ta") == 0) {
      get_task_values(agrc, agrv, &task_to_add);
      throw_errors();
      add_task(&task_to_add);
      throw_errors();
      return 0;
    }
  }
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-c") == 0)
    {
      show_categories();
      throw_errors();
    } else if(strcmp(agrv[i], "-t") == 0)
    {
      for(j=1;j<agrc;j++)
      {
        if(strcmp(agrv[j], "-l") == 0)
        {
          if(j+1 < agrc && agrv[j+1][0] != '-')
            limit = atoi(agrv[j+1]);
        } else if(strcmp(agrv[j], "-d") == 0)
        {
          desc = 1;
        } else if(strcmp(agrv[j], "-s") == 0)
        {
          if(j+1 < agrc && agrv[j+1][0] != '-')
            strncpy(sort_by, agrv[j+1], COL_TITLE_LEN);
        }
      }

      show_tasks(limit, sort_by, desc);
      throw_errors();
    }
  }
  return 0;
}

void get_category_values(int agrc, char * agrv[],  struct category * category_to_add)
{
  int i;
  char name[CATEGORY_LEN] = "";
  char id[ID_LEN] = "";
  
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-i") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
        strncpy(id, agrv[i+1], ID_LEN);
      else
        error("CATEGORY_NO_ID");
    } else if(strcmp(agrv[i], "-n") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
       strncpy(name, agrv[i+1], CATEGORY_LEN);
      else
        error("CATEGORY_NO_NAME");
    }
  }
  if(strcmp(id, "") == 0)
    error("CATEGORY_NO_ID");
  if(strcmp(name, "") == 0)
    error("CATEGORY_NO_NAME");
  
  strcpy(category_to_add->id, id);
  strcpy(category_to_add->name, name);
}

void get_task_values(int agrc, char * agrv[],  struct task * task_to_add)
{
  int i;
  struct tm date_struct;  
  regex_t date_format;  
  char id[ID_LEN] = "";
  char category[ID_LEN] = "";
  time_t date = -1;
  int time_needed = -1;
  char desc[DESC_LEN] = "";
  
  regcomp(&date_format, "^20[0-9][0-9]-[0-9][0-9]-[0-9][0-9]$", 0);  
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-i") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
        strncpy(id, agrv[i+1], ID_LEN);
    } else if(strcmp(agrv[i], "-c") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
      {
        if(driver_if_category_exists(agrv[i+1]))
        {
          strncpy(category, agrv[i+1], ID_LEN);
        } else
        {
          strncpy(category, "---", ID_LEN);
          error("TASK_NO_EXISTING_CATEGORY");
        }
      } else
      {
        error("TASK_NO_CATEGORY");
      }
    } else if(strcmp(agrv[i], "-d") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
      {
        if( ! regexec(&date_format, agrv[i+1], 0, NULL, 0) )
        {
          time (&date);
          date_struct = *localtime(&date);
          date_struct.tm_year = atoi(strtok(agrv[i+1], "-")) - 1900;
          date_struct.tm_mon = atoi(strtok(NULL, "-")) - 1;
          date_struct.tm_mday = atoi(strtok(NULL, "-"));
          date = mktime(&date_struct);
        } else
        {
          date = -2;
          error("TASK_DATE_WRONG_FORMAT");
        }
      }
      else
      {
        error("TASK_NO_DATE");
      }
    } else if(strcmp(agrv[i], "-t") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
        time_needed = atoi(agrv[i+1]);
      else
        error("TASK_NO_TIME_NEEDED");
    } else if(strcmp(agrv[i], "-e") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
        strncpy(desc, agrv[i+1], DESC_LEN);
      else
        error("TASK_NO_DESC");
    }
  }
  if(strcmp(id, "") == 0)
  {
    
  }
  if(strcmp(category, "") == 0)
    error("TASK_NO_CATEGORY");
  if(date == -1)
    error("TASK_NO_DATE");
  if(time_needed == -1)
    error("TASK_NO_TIME_NEEDED");
  if(strcmp(desc, "") == 0)
    error("TASK_NO_DESC");
  strcpy(task_to_add->id, id);
  strcpy(task_to_add->category, category);
  task_to_add->date = date;
  task_to_add->time_needed = time_needed;
  strcpy(task_to_add->desc, desc);
}

/**
 * Add new category.
 * 
 * @param id string - id of new category
 * @param name string - name of new category
 * 
 * Function prints succesful added message(if OUTPUT_FORMAT == STDOUT) or as a json object with new added category: {"category_id", "category_name"}
 * If an errors occours it prints cannot_modify_data error.
 */
void add_category(struct category * category_to_add)
{
  if(driver_add_category(category_to_add) == 1)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      puts("New category added..");
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      printf("{\"id\": \"%s\",\"name\": \"%s\"}", category_to_add->id, category_to_add->name);
    }
  } else
  {
    throw_errors();
  }
}

void add_task(struct task * task_to_add)
{
  puts(task_to_add->id);
  puts(task_to_add->category);
  printf("%d\n", (int)task_to_add->date);
  printf("%d\n", task_to_add->time_needed);
  puts(task_to_add->desc);
}

/**
 * Show categories of tasks.
 *
 * Function prints list of tasks as an ansi table(if OUTPUT_FORMAT == STDOUT) or as a json object: {{"category_id", "category_name"}, {"category_id", "category_name"}}
 * If an errors occours it prints cannot_acces_data error.
 */
void show_categories()
{
  puts("Cat");
}

void show_tasks(int limit, char * sort_by, int desc)
{
  puts(sort_by);
  printf("%d", limit);
}

void throw_errors()
{
  int i;
  if(ERR_NO != 0)
  {
    if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
      fprintf(stderr, "{");
    for(i=0;i<ERR_NO;i++)
    {
      if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
        fprintf(stderr, "\"%d\":", i);
      fprintf(stderr, "%s\n", ERRORS[i]);
      if(strcmp(OUTPUT_FORMAT, "JSON") == 0 && i+1 != ERR_NO)
        fprintf(stderr, ",");
    }
    if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
      fprintf(stderr, "}");
    exit(1);
  }
}

/**
 * Add error to ERRORS array based on error_code.
 *
 * Output can be presented as standard error message(if OUTPUT_FORMAT is set as STDOUT) or as a JSON output(if OUTPUT_FORMAT is set as JSON). JOSON format is:
 * {
 *  "type": "error",
 *  "component": "categories" or "tasks",
 *  "action": "add" or "update" or "remove" or "show",
 *  "code": error message code - avalible massages are given in the descripitonm of other application functions
 *  }
 * @param err_code string
 */
void error(char * err_code)
{
  if( ! in_array(err_code, ERRORS, ERR_NO))
  {
  if(strcmp(err_code, "CATEGORY_NO_ID") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "You don't provide id of category.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"categories\",\"action\": \"add\", \"code\": \"no_id\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "CATEGORY_NO_NAME") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "You don't provide name of category.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"categories\",\"action\": \"add\", \"code\": \"no_name\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "TASK_NO_CATEGORY") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "You don't provide category id.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"no_category\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "TASK_NO_EXISTING_CATEGORY") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "There is no category with id you passed.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"no_existing_category\"}";
      ERR_NO++;
    }    
  } else if(strcmp(err_code, "TASK_DATE_WRONG_FORMAT") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "Date should have format: yyyy-mm-dd.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"date_wrong_format\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "TASK_NO_DATE") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "You don't provide date of the task.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"no_date\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "TASK_NO_TIME_NEEDED") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] = "You don't specify the execution time.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] = "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"no_time_needed\"}";
      ERR_NO++;
    }
  } else if(strcmp(err_code, "TASK_NO_DESC") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
      ERRORS[ERR_NO] =  "You don't provide descrpition of the task.";
      ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
      ERRORS[ERR_NO] =  "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add\", \"code\": \"no_description\"}";
      ERR_NO++;
    }
  }
  }
}
char * upper(char * str)
{
  int i;
  for(i=0;i<strlen(str);i++)
  {
    str[i] = toupper(str[i]);
  }
  return str;
}
int in_array(char * str, char * array[], int len)
{
  int i;
  for(i=0;i<len;i++)
  {
    if(strcmp(str, array[i]) == 0)
      return 1;
  }
  return 0;
}
void set_output_format(char * format)
{
	if(in_array(upper(format), AVAILABLE_FORMATS, MAX_FORMATS))
	  strcpy(OUTPUT_FORMAT, format);
}
