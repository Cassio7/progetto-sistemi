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

struct aereo {
  int id;
  int numero;
};

static char end[10] = "\033[0m\n";

/*struct coda{
  struct aereo* aereo;
  struct pila* next;
};*/

static void stampevent(char *processo){
  char s[256];
  time_t timet;
  time(&timet);
  struct tm *pTm = localtime(&timet);
  sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
  printf("%s %s",s,processo);
}