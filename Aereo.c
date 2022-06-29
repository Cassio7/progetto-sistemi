#include "def.h"


int rangerand(int min, int max){
  srand(time(NULL));
  return (random() % (max - min +1)) + min;
}

int child(pid_t pid,int i,int pidT){
  int fd = open("/tmp/myfifo", O_RDWR);
  printf("\n");
  stampevent(AEREO);
  printf("Inizio del processo Aereo%s",END);
  struct aereo aereo = {pid,i};
  sigset_t sigset;
  int signum = 0;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  stampevent(AEREO);
  printf("Aereo numero = %d, id = %d%s",aereo.numero,aereo.id,END);
  int r = rangerand(3,8);
  stampevent(AEREO);
  printf("Inizio preparazione Aereo numero = %d, durata = %d%s",aereo.numero,r,END);
  sleep(r);
  //invio segnale per verificare disponibilità
  stampevent(AEREO);
  printf("Avvenuto invio alla Torre richiesta decollo da Aereo numero = %d%s",aereo.numero,END);
  if(write(fd, &aereo, sizeof(aereo)) == -1) {
    perror("Aereo: Errore in write");
    return 1;
  }
  sigwait(&sigset,&signum);
  //aspettare segnale per partire risposta Torre
  //inizio decollo
  r = rangerand(5,15);
  stampevent(AEREOCOL);
  printf("Decollo Aereo numero = %d, durata volo = %d%s",aereo.numero,r,END);
  sleep(r);
  //fine volo
  stampevent(AEREOCOL);
  printf("Fine decollo Aereo numero = %d%s",aereo.numero,END);
  kill(pidT,signum);
  //liberare pista
  close(fd);
  return 0;
}
