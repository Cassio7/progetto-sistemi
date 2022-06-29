#include "def.h"

void child(pid_t,int,int);

int main() {
  stampevent(HANGAR);
  printf("Creazione del processo Hangar%s",END);
  mkfifo("/tmp/myfifo", S_IRWXU);
  int fd = open("/tmp/myfifo", O_RDWR),pidT,status,numaer = 10;
  if(read(fd, &pidT, sizeof(pidT)) == -1) {
    perror("HANGAR: Errore in read");
    return 1;
  }
  do {
    stampevent(HANGAR);
    printf("Inserisci il numero di Aerei da far decollare \n(maggiore o uguale a 10, minore o uguale a 20): %s",END);
    scanf("%d",&numaer);
  } while(numaer < 10 || numaer > 20);
  pid_t pid[numaer];

  if(write(fd, &numaer, sizeof(numaer)) == -1) {
    perror("HANGAR: Errore in write");
    return 1;
  }

  stampevent(HANGAR);
  printf("Inizio creazione di ogni aereo%s",END);
  for (int i = 0; i < numaer; i++) { //for per la creazione degl'aerei
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
  for (int i = 0; i < numaer; i++) {
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
