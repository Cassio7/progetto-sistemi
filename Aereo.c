#include "def.h"

char Aereo[30] = "Aereo: ";



int rangerand(int min, int max){
  srand(time(NULL));
  return (random() % (max - min +1)) + min;
}

int child(pid_t pid,int i,int pidT){
  int fd = open("/tmp/myfifo", O_RDWR);

  stampevent(Aereo);
  printf("Inizio del processo Aereo%s",end);
  struct aereo aereo = {pid,i};
  sigset_t sigset;
  int signum = 0;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  stampevent(Aereo);
  printf("Aereo numero = %d, id = %d%s",aereo.numero,aereo.id,end);
  int r = rangerand(3,8);
  stampevent(Aereo);
  printf("Inizio preparazione Aereo numero = %d, durata = %d%s",aereo.numero,r,end);
  sleep(r);
  //invio segnale per verificare disponibilità
  stampevent(Aereo);
  printf("Avvenuto invio alla Torre richiesta decollo da Aereo numero = %d%s",aereo.numero,end);
  if(write(fd, &aereo, sizeof(aereo)) == -1) {
    perror("Aereo: Errore in write");
    return 1;
  }
  sigwait(&sigset,&signum);
  //aspettare segnale per partire risposta Torre
  //inizio decollo
  r = rangerand(5,15);
  stampevent(Aereo);
  printf("Decollo Aereo numero = %d, durata volo = %d%s",aereo.numero,r,end);
  sleep(r);
  //fine volo
  stampevent(Aereo);
  printf("Fine decollo Aereo numero = %d%s",aereo.numero,end);
  /*if(read(fd, &pidT, sizeof(pidT)) == -1) {
    perror("Aereo: Errore in read");
    return 1;
  }*/
  kill(pidT,signum);
  //liberare pista
  close(fd);
  return 0;
}
