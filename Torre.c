#include "def.h"


static bool pista1=false,pista2=false; //false = disponibile, true = non disponibile
struct aereo aereo; //struttura aereo di appoggio


int main() {
  int i = 0,pidT = getpid();//i contatore aerei , pidT num processo torre
  sigset_t sigset;
  int signum = 0;
  sleep(1);
  int fd = open("/tmp/myfifo", O_RDWR); //apertura pipe
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  stampevent(TORRE);
  printf("Avvio del processo Torre%s",END);

 //invio tramite pipe il pid di torre ad hangar
  if(write(fd, &pidT, sizeof(pidT)) == -1) {
    perror("Torre: Errore in write");
    exit(1);
  }

  //ciclo do while principalei finisce quando arrivano 10 aerei
  do {
    if(read(fd, &aereo, sizeof(aereo)) == -1) { //riceve da pipe dati aereo
      perror("Torre: Errore in read");
      return 1;
    }
    stampevent(TORRE);
    printf("Richiesta di decollo da Aereo numero = %d%s",aereo.numero,END);
    i++; //conta gli aerei
    if (!pista1) { //se pista è libera entra
      stampevent(TORRE);
      printf("Autorizzazione al decollo Aereo numero = %d, da pista 1%s",aereo.numero,END);
      pista1 = true;
      kill(aereo.id,SIGRTMIN + 1);
    }
    else if (!pista2) {
      stampevent(TORRE);
      printf("Autorizzazione al decollo Aereo numero = %d, da pista 2%s",aereo.numero,END);
      pista2 = true;
      kill(aereo.id,SIGRTMIN + 2);
    }
    else {
      sigwait(&sigset,&signum);
      if (signum == (SIGRTMIN + 1) ){
          pista1 = false;
          printf("liberata pista 1\n");
          stampevent(TORRE);
          printf("Autorizzazione al decollo Aereo numero = %d, da pista 1%s",aereo.numero,END);
          pista1 = true;
          kill(aereo.id,SIGRTMIN + 1);
      }
      else if(signum == (SIGRTMIN + 2)){
          printf("liberata pista 2\n");
          pista2 = false;
          stampevent(TORRE);
          printf("Autorizzazione al decollo Aereo numero = %d, da pista 2%s",aereo.numero,END);
          pista2 = true;
          kill(aereo.id,SIGRTMIN + 2);
      }
    }
  }while(i != 10);
  do{
      sigwait(&sigset,&signum);
  }while(signum != SIGRTMIN + 3);
  stampevent(TORRE);
  printf("Ricevuto segnale da Hangar, non ci sono piu' Aerei da far decollare%s",END);
  stampevent(TORRE);
  printf("Terminazione processo Torre%s",END);
  close(fd);
  return 0;
}
