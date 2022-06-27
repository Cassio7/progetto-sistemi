#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

void child(pid_t,int);

void stampevent(char *processo){
  char s[256];
  time_t timet;
  time(&timet);
  struct tm *pTm = localtime(&timet);
  sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
  printf("%s %s",s,processo);
}

int main() {
  stampevent("Hangar: ");
  printf("Creazione del processo Hangar\n");
  pid_t pid[10];

  stampevent("Hangar: ");
  printf("Inizio creazione di ogni aereo\n");
  for (int i = 1; i <= 10; i++) {
    pid[i] = fork();
    sleep(2);
    if (pid[i]<0)
      fprintf(stderr, "Fork fallito come me\n");
    else if (pid[i] == 0) {
      pid[i]= getpid();
      child(pid[i],i);
      exit(0);
    }
  }
  for (int i = 1; i <= 10; i++) {
    wait(NULL);
  }
  stampevent("Hangar: ");
  printf("Fine creazione di ogni aereo\n");
  return 0;
}
