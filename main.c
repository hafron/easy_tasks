#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "easy_tasks.h" 
#include "driver.h"


int get_struct(struct task *);

int main(int agrc, char * agrv[])
{
  if(agrc == 1)
  {
    char choise;
    struct task this_task;
    do
    {
      puts("********************");
      puts("|    Menu główne   |");
      puts("********************");
      puts("1 - dodaj nowe zadanie");
      puts("2 - dodaj nową kategorię");
      puts("3 - obejrzyj zadania");
      puts("4 - wyjdź");
      printf("Wybierz opcję[1-4]: ");
      choise = getchar();
      if(choise != '\n')
      {
        while(getchar() != '\n')
          continue;
      }
      switch(choise)
      {
        case '1':
          get_struct(&this_task);
          add_task(&this_task);
        break;
        case '4':
          exit(0);
        break;
      }
    } while(1);
  } else
  {
    puts("Tryb standardowy.");
  } 
  return 0;
}
int get_struct(struct task * this_task)
{
  char id[ID_LEN], category[CATEGORY_LEN], desc[DESC_LEN], chose, ch;
  int all_is_good=0, id_not_set = 0, year, mounth, day, minutes, i;
  while(all_is_good == 0)
  {
    printf("Podaj id zadania(<ESC>+<ENTER> - powróć do manu głównego)[puste jeżeli ma być przydzielone automatycznie]: ");
    fgets(id, ID_LEN, stdin);
    if(strcmp(id, "\033\n") == 0)
    {
      return 0;
    } else if(strcmp(id, "\n") == 0)
    {
      id_not_set = 1;
    }
    printf("Podaj kategorię: ");
    fgets(category, CATEGORY_LEN, stdin);
    puts("Podaj datę zakończenia:");
    printf("	rok: ");
    scanf("%d", &year);
    printf("	miesiąc: ");
    scanf("%d", &mounth);
    printf("	dzień: ");
    scanf("%d", &day);
    printf("Przewidywany czas wykonania zadania(minuty): ");
    scanf("%d", &minutes);
    puts("Opis zadania(<CRTL>+D+<ENTER> - zakończ wprowadzanie): ");
    i=0;
    while((ch = getchar()) != EOF && i < DESC_LEN)
    {
      desc[i] = ch;
      i++;
    }
    fgets(desc, DESC_LEN, stdin);
    printf("Czy podane informacje są poprawne?(t/n)");
    chose = getchar();
    while(getchar() != '\n')
      continue;
    if(chose == 't')
      all_is_good = 1;
  }
  
  return 1;
}


