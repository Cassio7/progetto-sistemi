#include "def.h"


static bool pista1=false,pista2=false; //false = disponibile, true = non disponibile
struct aereo aereo;
char Torre[30] = "\033[1;35mTorre: ";

int main() {
  int i = 0,pidT = getpid();
  sigset_t sigset;
  int signum = 0;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  stampevent(Torre);
  printf("Avvio del processo Torre%s",end);
  int fd = open("/tmp/myfifo", O_RDWR);
  if(write(fd, &pidT, sizeof(pidT)) == -1) {
    perror("Torre: Errore in write");
    exit(1);
  }
  do {
    if(read(fd, &aereo, sizeof(aereo)) == -1) {
      perror("Torre: Errore in read");
      return 1;
    }
    stampevent(Torre);
    printf("Richiesta di decollo da Aereo numero = %d%s",aereo.numero,end);
    i++; //conta gli aerei
    if (!pista1) {
      stampevent(Torre);
      printf("Autorizzazione al decollo Aereo numero = %d, da pista 1%s",aereo.numero,end);
      pista1 = true;
      kill(aereo.id,SIGRTMIN + 1);
    }
    else if (!pista2) {
      stampevent(Torre);
      printf("Autorizzazione al decollo Aereo numero = %d, da pista 2%s",aereo.numero,end);
      pista2 = true;
      kill(aereo.id,SIGRTMIN + 2);
    }
    else {
      sigwait(&sigset,&signum);
      if (signum == (SIGRTMIN + 1) ){
          pista1 = false;
          printf("liberata pista 1\n");
          stampevent(Torre);
          printf("Autorizzazione al decollo Aereo numero = %d, da pista 1%s",aereo.numero,end);
          pista1 = true;
          kill(aereo.id,SIGRTMIN + 1);
      }
      else if(signum == (SIGRTMIN + 2)){
          printf("liberata pista 2\n");
          pista2 = false;
          stampevent(Torre);
          printf("Autorizzazione al decollo Aereo numero = %d, da pista 2%s",aereo.numero,end);
          pista2 = true;
          kill(aereo.id,SIGRTMIN + 2);
      }
    }
  }while(i != 10);
  sigwait(&sigset,&signum);
  stampevent(Torre);
  printf("Ricevuto segnale da Hangar, non ci sono piu' Aerei da far decollare%s",end);
  stampevent(Torre);
  printf("Terminazione processo Torre%s",end);
  close(fd);
  return 0;
}
