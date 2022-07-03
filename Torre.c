#include "def.h"


static bool pista1=false,pista2=false; //false = disponibile, true = non disponibile
struct aereo aereo; //struttura aereo di appoggio

void permesso(short p){ //funzione per autorizzazione del volo
  stampevent(TORRE);
  printf("Autorizzazione al decollo Aereo numero = %d, da pista %hd%s",aereo.numero,p,END);
  if (p == 1) { //pista 1
    pista1 = true;
    //mando un segnale al pid = aereo.id, invio SIGRTMIN + 1 che identifica pista 1
    kill(aereo.id,SIGRTMIN + 1);
  }
  else if (p == 2) {//pista 2
    pista2 = true;
    //mando un segnale al pid = aereo.id, invio SIGRTMIN + 2 che identifica pista 2
    kill(aereo.id,SIGRTMIN + 2);  }
}

int main() {
  sigset_t sigset; //set of signals
  //signum specifica il signal di cui variare gestione, numaer numero aerei
  int signum = 0,numaer = 10,pidT = getpid();//pidT pid processo torre
  short p = 0; //identificatore pista funzione
  sleep(1); //sleep per far partire prima Hangar
  int fd = open("/tmp/myfifo", O_RDWR); //apertura pipe
  if (fd == -1) { //catturo errore pipe
    perror("Torre: Errore in fd");
    return 1;
  }
  //mascheramento della macro SIGRTMIN -> Real-Time signal
  sigemptyset(&sigset); //inizializzo il set di segnali a vuoto
  //aggiungo al set le seguenti macro
  sigaddset(&sigset, SIGRTMIN + 1);
  sigaddset(&sigset, SIGRTMIN + 2);
  sigaddset(&sigset, SIGRTMIN + 3);
  sigprocmask(SIG_BLOCK, &sigset, NULL); //unisce il blocco di segnali al set,NULL = oldset

  stampevent(TORRE);
  printf("Avvio del processo Torre%s",END);
 //invio tramite pipe il pid di torre ad hangar
  if(write(fd, &pidT, sizeof(pidT)) == -1) {
    perror("Torre: Errore in write");
    exit(1);
  }
  printf("--------TORRE: WRITE FATTA %d----------\n",pidT);
  close(fd); //chiudo pipe per non inviare il pid alla read sotto
  sigwait(&sigset,&signum);//uso pure una wait per lo stesso motivo sopra

  fd = open("/tmp/myfifo", O_RDWR); //apertura pipe
  if (fd == -1) {
    perror("Torre: Errore in fd");
    return 1;
  }
  //leggo il numero di aerei tramite pipe che arriva da hangar
  if(read(fd, &numaer, sizeof(numaer)) == -1) {
    perror("Torre: Errore in read");
    exit(1);
  }
  printf("--------TORRE: READ FATTA %d----------\n",numaer);
  //ciclo for principale finisce quando i=numaer aerei
  for(int i = 0; i < numaer;i++){
    if(read(fd, &aereo, sizeof(aereo)) == -1) { //riceve da pipe dati aereo
      perror("Torre: Errore in read");
      return 1;
    }
    printf("\n");
    stampevent(TORRE);
    printf("Richiesta di decollo da Aereo numero = %d%s",aereo.numero,END);
    if (!pista1) { //se pista 1 è libera entra
      p = 1; //pista 1 per funzione
      permesso(p); //richiamo la funzione di permesso volo e passo il numero pista
    }
    else if (!pista2) { //se pista 2 è libera entra
      p = 2; //pista 2 per funzione
      permesso(p);
    }
    //se tutte le piste sono occupate l'aereo va in una coda di attesa e aspetta il turno
    else {
      //torre attende il segnale da aereo che libera la pista dopo decollo
      sigwait(&sigset,&signum);
      //tramite SIGRTMIN + 1 capisco che si è liberata pista 1
      if (signum == (SIGRTMIN + 1) ){
          pista1 = false;//pista libera
          stampevent(TORRE);
          printf("Liberata pista 1%s",END);
          p = 1;
          permesso(p);
      }
      //tramite SIGRTMIN + 2 capisco che si è liberata pista 2
      else if(signum == (SIGRTMIN + 2)){
          pista2 = false;
          stampevent(TORRE);
          printf("Liberata pista 2%s",END);
          p = 2;
          permesso(p);
      }
    }
  }
  //attendo il segnale di chiusura da hangar, uso do while per far finire torre dopo hangar
  do{
      sigwait(&sigset,&signum);
  }while(signum != SIGRTMIN + 3); //deve essere il SIGRTMIN + 3
  stampevent(TORRE);
  printf("Ricevuto segnale da Hangar, non ci sono piu' Aerei da far decollare%s",END);
  stampevent(TORRE);
  printf("Terminazione processo Torre%s",END);
  close(fd); //chiudo la pipe
  return 0;
}
