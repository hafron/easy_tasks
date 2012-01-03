#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sqlite3.h>
#include "easy_tasks.h" 
#include "sqlite_config.h" 

extern char OUTPUT_FORMAT[MAX_FORMAT_LEN];
extern char * ERRORS[MAX_ERRORS];
extern int ERR_NO;

sqlite3_stmt * STATEMENT;
sqlite3 * DB;

int driver_connect(sqlite3 ** db) {
  
    int rc;
    rc = sqlite3_open(SQLITE_DATABASE_FILE, db);
    if( rc ){
      if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
      {
        sprintf(ERRORS[ERR_NO], "Can't open database: %s", sqlite3_errmsg(*db));
        ERR_NO++;
      } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
      {
        sprintf(ERRORS[ERR_NO], "{\"type\": \"error\",\"component\": \"tasks\",\"action\": \"add_sql\", \"code\": \"%s\"}", sqlite3_errmsg(*db));
        ERR_NO++;
      }
      sqlite3_close(*db);
      return 1;
    }
    return 0;
}
int error_query(sqlite3 * db, char * query, int (*callback)(void*,int,char**,char**))
{
  char *zErrMsg = 0;
  char err[1000];
  int rc;
  
  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
        sprintf(err, "SQL error: %s", zErrMsg);
        ERRORS[ERR_NO] = err;
        ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
        sprintf(err, "{\"type\": \"error\",\"component\": \"categories\",\"action\": \"add_sql\", \"code\": \"%s\"}", zErrMsg);
        ERRORS[ERR_NO] = err;
        ERR_NO++;
    }
    sqlite3_free(zErrMsg);
    return 1;
  }
  return 0;
}

int error_prepare(sqlite3 * db, char * query, sqlite3_stmt ** statement)
{
  
  int rc;
  
  rc = sqlite3_prepare_v2(db, query, -1, statement, 0);
  if(rc != SQLITE_OK)
  {
    if(strcmp(OUTPUT_FORMAT, "STDOUT") == 0)
    {
        sprintf(ERRORS[ERR_NO], "SQL error: %s", sqlite3_errmsg(db));
        ERR_NO++;
    } else if(strcmp(OUTPUT_FORMAT, "JSON") == 0)
    {
        sprintf(ERRORS[ERR_NO], "{\"type\": \"error\",\"component\": \"categories\",\"action\": \"select_sql\", \"code\": \"%s\"}", sqlite3_errmsg(db));
        ERR_NO++;
    }
    return 1;
  }
  return 0;
}

int driver_add_task(struct task * task_to_add)
{
  char query[10000];    
    
  if(driver_connect(&DB))
    return 0;
    
  sprintf(query, "INSERT INTO tasks(id, category, date, time_needed, desc) VALUES (\"%s\", \"%s\", \"%d\", \"%d\", \"%s\")", task_to_add->id, task_to_add->category, (int)task_to_add->date, task_to_add->time_needed, task_to_add->desc);

  if(error_query(DB, query, NULL))
    return 0;
  
  sqlite3_close(DB);
  return 1;
}
int driver_add_category(struct category * category_to_add)
{
  char query[1000];    
    
  if(driver_connect(&DB))
    return 0;
    
  sprintf(query, "INSERT INTO categories(id, name) VALUES (\"%s\", \"%s\")", category_to_add->id, category_to_add->name);
  if(error_query(DB, query, NULL))
    return 0;
  
  sqlite3_close(DB);
  return 1;
}
/*
 0 =error
 1 - no_errors
*/
int driver_category_init()
{
  char query[1000];
  
  if(driver_connect(&DB))
    return 0;
  
  sprintf(query, "SELECT * FROM categories");
  if(error_prepare(DB, query, &STATEMENT))
  {
    sqlite3_finalize(STATEMENT);
    sqlite3_close(DB);
    return -1;
  }
  
  return 1;
}
/*1- next row exists
  0 - yu reach end of the function
*/
int driver_category_next(char row[2][CATEGORY_LEN])
{
  int result;
  result = sqlite3_step(STATEMENT);
			
  if(result == SQLITE_ROW)
	{
    strcpy(row[0], (char *)sqlite3_column_text(STATEMENT, 0));
    strcpy(row[1], (char *)sqlite3_column_text(STATEMENT, 1));
		return 1;
	}
	else
	{
    sqlite3_finalize(STATEMENT);
		return 0;
	}
}
/*
 1 - yes
 0 - no
 -1 -err
*/
int driver_if_category_exists(char * id)
{
  char query[1000];
  
  if(driver_connect(&DB))
    return 0;
  
  sprintf(query, "SELECT id FROM categories WHERE id = \"%s\"", id);
  if(error_prepare(DB, query, &STATEMENT))
  {
    sqlite3_finalize(STATEMENT);
    sqlite3_close(DB);
    return -1;
  }
  sqlite3_close(DB);
  if(sqlite3_step(STATEMENT) == SQLITE_ROW)
  {
    sqlite3_finalize(STATEMENT);
    return 1;
  }
  else
  {
    sqlite3_finalize(STATEMENT);
    return 0;
  }
}