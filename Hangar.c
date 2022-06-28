#include "def.h"


void child(pid_t,int);

int main() {
  int status;
  stampevent("Hangar: ");
  printf("Creazione del processo Hangar\n");
  pid_t pid[10];
  mkfifo("/tmp/myfifo", S_IRWXU);

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
  unlink("/tmp/myfifo");
  return 0;
}
