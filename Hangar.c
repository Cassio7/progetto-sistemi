#include "def.h"

void child(pid_t,int,int); //dichiarazione della funzione child presente in aereo

int main() {
  system("clear"); //pulisco terminale tramite comando
  mkfifo("/tmp/myfifo", S_IRWXU); //creazione della pipe
  stampevent(HANGAR);
  printf("Creazione del processo Hangar%s",END);
  int pidT,status,numaer = 10; //pidT = pid torre, status = serve per wait child, numaer = numero aerei
  int fd = open("/tmp/myfifo", O_RDWR); //apertura pipe
  if (fd == -1) { //controllo apertura pipe
    perror("HANGAR: Errore in fd");
    return 1;
  }
  //leggo tramite pipe il pid che arriva da torre
  if(read(fd, &pidT, sizeof(pidT)) == -1) {
    perror("HANGAR: Errore in read");
    return 1;
  }
  close(fd);
  kill(pidT,SIGRTMIN + 3); //sblocca torre dallo wait per evitare di inviare il pid alla read sotto in torre

  //do while per inserimento numero aerei che va da 10 a 20 compresi
  do {
    stampevent(HANGAR);
    printf("Inserisci il numero di Aerei da far decollare \n(maggiore o uguale a 10, minore o uguale a 20): %s",END);
    scanf("%d",&numaer);
  } while(numaer < 10 || numaer > 20);
  pid_t pid[numaer]; //dichiarazione dei pid in base al numaer

  fd = open("/tmp/myfifo", O_RDWR); //apertura pipe
  if (fd == -1) { //controllo apertura pipe
    perror("HANGAR: Errore in fd");
    return 1;
  }
  // passo tramite pipe il numero di aerei a torre
  if(write(fd, &numaer, sizeof(numaer)) == -1) {
    perror("HANGAR: Errore in write");
    return 1;
  }
  close(fd);//chiudo pipe
  stampevent(HANGAR);
  printf("Inizio creazione di ogni aereo%s",END);
  for (int i = 0; i < numaer; i++) { //for per la creazione degl'aerei
    pid[i] = fork(); //creazione del figlio
    if (pid[i]<0) //controllo se li fork è fallito
      fprintf(stderr, "Fork fallito\n");
    else if (pid[i] == 0) { //fork andato a buon fine
      pid[i]= getpid(); //catturo pid del processo creato
      child(pid[i],i+1,pidT); //passo tutti i dati al figlio, pure il pid di torre
      exit(0); //termina il processo chiamante
    }
    sleep(2); //metto in pausa il processo per 2 secondi
  }
  for (int i = 0; i < numaer; i++) {
    wait(&status); //sospendo il chiamante e attendo finche' un child termina
  }
  stampevent(HANGAR);
  printf("Fine creazione di ogni aereo%s",END);
  unlink("/tmp/myfifo");//rimuovo la pipe
  stampevent(HANGAR);
  printf("Non ci sono piu' Aerei da far decollare, invio segnale a Torre%s",END);
  kill(pidT,SIGRTMIN + 3); //mando segnale di fine aerei a torre
  stampevent(HANGAR);
  printf("Fine processo Hangar%s",END);
  return 0;
}
