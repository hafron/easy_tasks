#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "easy_tasks.h" 
#include "driver.h"

/**
 * OUTPUT_FORMAT defines format of output. Current avalible values are "STDOUT" - for text mode usage and JSON - if you want to get output in JSON format.
 */
char OUTPUT_FORMAT[7] = "STDOUT";

/**
 * Add new category.
 * 
 * @param id string - id of new category
 * @param name string - name of new category
 * 
 * @return record succesful added message(if OUTPUT_FORMAT == STDOUT) or as a json object with new added category: {"category_id", "category_name"}
 * If an errors occours it returns cannot_modify_data error.
 */
int add_category(const struct category *);

/**
 * Show categories of tasks.
 *
 * @return list of tasks as an ansi table(if OUTPUT_FORMAT == STDOUT) or as a json object: {{"category_id", "category_name"}, {"category_id", "category_name"}}
 * If an errors occours it returns cannot_acces_data error.
 */
int show_categories();

/**
 * Return error output based on error_code.
 *
 * Output can be presented as standard error message(if OUTPUT_FORMAT is set as STDOUT) or as a JSON output(if OUTPUT_FORMAT is set as JSON). JOSON format is:
 * {
 *  "type": "error",
 *  "component": "categories" or "tasks",
 *  "action": "add" or "update" or "remove" or "show",
 *  "code": error message code - avalible massages are given in the descripitonm of other application functions
 *  }
 * @param err_code string
 * @return int error code. First number represents component, second represents action:
 * Components:
 *   1 - categories
 *   2 - tasks
 * Actions:
 *   1 - add
 *   2 - update
 *   3 - remove
 *   4 - show
 */
int error(char *);

void get_category_values(int, char **, struct category *);

/**
 * Function reads console params and calls appropriate functions.
 * 
 */
int main(int agrc, char * agrv[])
{
  int i, j;
  struct category category_to_add;
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-a") == 0)
    {
      for(j=1;j<agrc;j++)
      {
        if(strcmp(agrv[j], "-c") == 0)
        {
          get_category_values(agrc, agrv, &category_to_add);
          return add_category(&category_to_add);
        }
      }
    } else if(strcmp(agrv[i], "-ac") == 0 || strcmp(agrv[i], "-ca") == 0) {
      get_category_values(agrc, agrv, &category_to_add);
      return add_category(&category_to_add);
    }
  }
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-c") == 0)
      return show_categories();
  }
  return 0;
}

void get_category_values(int agrc, char * agrv[],  struct category * category_to_add)
{
  int i;
  for(i=1;i<agrc;i++)
  {
    if(strcmp(agrv[i], "-i") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
        strcpy(category_to_add->id, agrv[i+1]);
      else
        error("CATEGORY_NO_ID");
    } else if(strcmp(agrv[i], "-n") == 0)
    {
      if(i+1 < agrc && agrv[i+1][0] != '-')
       strcpy(category_to_add->name, agrv[i+1]);
      else
        error("CATEGORY_NO_NAME");
    }
  }
}



int add_category(const struct category * category_to_add)
{
  puts(category_to_add->id);
  puts(category_to_add->name);
  return 0;
}

int show_categories()
{
  
  return 1;
}

int error(char * err_code)
{
  if(strcmp(err_code, "CATEGORY_NO_ID") == 0)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
      puts("You don't provide name of category.");
    else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
      puts("{\"type\": \"error\",\"component\": \"categories\",\"action\": \"add\", \"code\": \"no_id\"}");
    return 10;
  }
  
  return 1;
}