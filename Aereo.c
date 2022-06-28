#include "def.h"


int rangerand(int min, int max){
  srand(time(NULL));
  return (random() % (max - min +1)) + min;
}

void child(pid_t pid,int i){
  stampevent("Aereo: ");
  printf("Inizio del processo Aereo\n");
  struct aereo aereo = {pid,i};

  stampevent("Aereo: ");
  printf("Aereo numero = %d, id = %d\n",aereo.numero,aereo.id );
  int r = rangerand(3,8);
  stampevent("Aereo: ");
  printf("Inizio preparazione Aereo numero = %d, durata = %d\n",aereo.numero,r);
  sleep(r);
  //invio segnale per verificare disponibilità
  int fd = open("/tmp/myfifo", O_WRONLY);
  if(write(fd, &aereo, sizeof(aereo)) == -1) {
    perror("Child: Errore in write");
    exit(1);
  }
  stampevent("Aereo: ");
  printf("Avvenuto invio alla Torre richiesta decollo da Aereo numero = %d\n",aereo.numero);
  pause();
  //aspettare segnale per partire risposta Torre
  //inizio decollo
  r = rangerand(5,15);
  stampevent("Aereo: ");
  printf("Decollo Aereo numero = %d, durata volo = %d\n",aereo.numero,r);
  sleep(r);
  //fine volo
  stampevent("Aereo: ");
  printf("Arrivato Aereo numero = %d\n",aereo.numero);
  //liberare pista
  close(fd);
}
