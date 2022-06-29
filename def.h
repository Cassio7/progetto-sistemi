#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>

//\033 escape sequence
#define END "\033[0m\n" //[0m torna normale
#define HANGAR "\033[1;34mHangar: "//[1;34 = grassetto e blu
#define AEREO "Aereo: "
#define AEREOCOL "\033[1;31mAereo: "//[1;34 = grassetto e rosso
#define TORRE "\033[1;35mTorre: "//[1;34 = grassetto e viola

struct aereo { //struttura aereo id = pid processo aereo, numero = da 1 a 10
  int id;
  int numero;
};

//permette di stampare orario ad ogni azione di ogni processo
static void stampevent(char *processo){
  char s[256];
  time_t timet;
  time(&timet);
  struct tm *pTm = localtime(&timet);
  sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
  printf("%s %s",s,processo);
}
