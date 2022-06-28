#include "def.h"


void child(pid_t,int,int);
char Hangar[30] = "\033[1;34mHangar: ";

int main() {
  int status;
  stampevent(Hangar);
  printf("Creazione del processo Hangar%s",end);
  pid_t pid[10];
  mkfifo("/tmp/myfifo", S_IRWXU);
  int fd = open("/tmp/myfifo", O_RDONLY),pidT;
  if(read(fd, &pidT, sizeof(pidT)) == -1) {
    perror("Hangar: Errore in read");
    return 1;
  }
  stampevent(Hangar);
  printf("Inizio creazione di ogni aereo%s",end);
  for (int i = 0; i < 10; i++) { //for per la creazione degl'aerei
    pid[i] = fork(); //creazione del figlio
    if (pid[i]<0)
      fprintf(stderr, "Fork fallito\n");
    else if (pid[i] == 0) {
      pid[i]= getpid();
      child(pid[i],i+1,pidT);
      exit(0);
    }
    sleep(2);
  }
  for (int i = 0; i < 10; i++) {
    wait(&status);
  }
  stampevent(Hangar);
  printf("Fine creazione di ogni aereo%s",end);
  unlink("/tmp/myfifo");
  stampevent(Hangar);
  printf("Non ci sono piu' Aerei da far decollare, invio segnale a Torre%s",end);
  kill(pidT,SIGRTMIN + 3);
  stampevent(Hangar);
  printf("Fine processo Hangar%s",end);
  return 0;
}
