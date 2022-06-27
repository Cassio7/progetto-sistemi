#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
  int status;
  stampevent("Hangar: ");
  printf("Creazione del processo Hangar\n");
  pid_t pid[10];

  stampevent("Hangar: ");
  printf("Inizio creazione di ogni aereo\n");
  for (int i = 0; i < 10; i++) { //for per la creazione degl'aerei
    pid[i] = fork(); //creazione del figlio
    if (pid[i]<0)
      fprintf(stderr, "Fork fallito\n");
    else if (pid[i] == 0) {
      pid[i]= getpid();
      child(pid[i],i+1);
      exit(0);
    }
    sleep(2);
  }
  for (int i = 0; i < 10; i++) {
    wait(&status);
  }
  stampevent("Hangar: ");
  printf("Fine creazione di ogni aereo\n");
  stampevent("Hangar: ");
  printf("Fine processo Hangar\n");
  return 0;
}
