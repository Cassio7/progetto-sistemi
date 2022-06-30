#include "def.h"


//funzione creazione numero random
int rangerand(int min, int max){
    static int bSeed = 0;
    if(!bSeed) {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      srandom(tv.tv_usec % 1000);
      bSeed = 1;
    }
  return (random() % (max - min + 1)) + min; //ritorno il numero random
}

//funzione creazione di un processo aereo figlio di hangar
int child(pid_t pid,int i,int pidT){ //prendo pid aereo, numero aereo, pidT = pid torre
  int fd = open("/tmp/myfifo", O_RDWR); //apro pipe
  if (fd == -1) { //controllo pipe aperta
    perror("Aereo: Errore in fd");
    return 1;
  }
  printf("\n");
  stampevent(AEREO);
  printf("Inizio del processo Aereo%s",END);
  struct aereo aereo = {pid,i}; //creo struct aereo mettendo il pid e il numero aereo
  //parte di gestione dei segnali, identica a torre
  sigset_t sigset;
  int signum = 0;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  stampevent(AEREO);
  printf("Aereo numero = %d, id = %d%s",aereo.numero,aereo.id,END);
  int r = rangerand(3,8); //genero numero random tra 3 e 8 compresi
  stampevent(AEREO);
  printf("Inizio preparazione Aereo numero = %d, durata = %d%s",aereo.numero,r,END);
  sleep(r); //metto in pausa per r secondi questo processo
  //invio i dati aereo alla torre tramite pipe
  if(write(fd, &aereo, sizeof(aereo)) == -1) { //controllo scrittura corretta
    perror("Aereo: Errore in write");
    return 1;
  }
  stampevent(AEREO);
  printf("Avvenuto invio alla Torre richiesta decollo da Aereo numero = %d%s",aereo.numero,END);
  sigwait(&sigset,&signum);//aspetta segnale per partire risposta Torre
  //inizio decollo
  r = rangerand(5,15); //numero random tra 5 e 15 compresi
  stampevent(AEREOCOL);
  printf("Decollo Aereo numero = %d, durata volo = %d%s",aereo.numero,r,END);
  sleep(r);
  //fine volo
  stampevent(AEREOCOL);
  printf("Fine decollo Aereo numero = %d%s",aereo.numero,END);
  //mando un segnale a torre tramite pidT, signum = SIGRTMIN + 1 or 2 in base alla pista usata
  //e libero la pista
  kill(pidT,signum);
  close(fd); //chiudo pipe
  return 0;
}
