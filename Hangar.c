#include "def.h"


void child(pid_t,int);
char Hangar[30] = "\033[1;34mHangar: ";

int main() {
  int status;
  stampevent(Hangar);
  printf("Creazione del processo Hangar%s",end);
  pid_t pid[10];
  mkfifo("/tmp/myfifo", S_IRWXU);

  stampevent(Hangar);
  printf("Inizio creazione di ogni aereo\033[0m\n");
  for (int i = 0; i < 1; i++) { //for per la creazione degl'aerei
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
  for (int i = 0; i < 1; i++) {
    wait(&status);
  }
  stampevent(Hangar);
  printf("Fine creazione di ogni aereo\033[0m\n");
  stampevent(Hangar);
  printf("Fine processo Hangar\033[0m\n");
  unlink("/tmp/myfifo");
  return 0;
}
