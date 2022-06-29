#include "def.h"

void child(pid_t,int,int);

int main() {
  int status;
  pid_t pid[10];

  stampevent(HANGAR);
  printf("Creazione del processo Hangar%s",END);
  mkfifo("/tmp/myfifo", S_IRWXU);
  int fd = open("/tmp/myfifo", O_RDONLY),pidT;
  if(read(fd, &pidT, sizeof(pidT)) == -1) {
    perror("HANGAR: Errore in read");
    return 1;
  }
  stampevent(HANGAR);
  printf("Inizio creazione di ogni aereo%s",END);
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
  stampevent(HANGAR);
  printf("Fine creazione di ogni aereo%s",END);
  close(fd);
  unlink("/tmp/myfifo");
  stampevent(HANGAR);
  printf("Non ci sono piu' Aerei da far decollare, invio segnale a Torre%s",END);
  kill(pidT,SIGRTMIN + 3);
  stampevent(HANGAR);
  printf("Fine processo Hangar%s",END);
  return 0;
}
