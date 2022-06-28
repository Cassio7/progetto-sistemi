#include "def.h"


bool pista1=false,pista2=false; //false = disponibile, true = non disponibile
int coda[10];
struct aereo aereo;

int main() {
  int i = 0;
  stampevent("Torre: ");
  printf("Avvio del processo Torre\n");
  int fd = open("/tmp/myfifo", O_RDONLY);
  if(read(fd, &aereo, sizeof(aereo)) == -1) {
    perror("Parent: Errore in read");
    return 1;
  }
  coda[i] = aereo.id;
  stampevent("Torre: ");
  printf("Richiesta di decollo da Aereo numero = %d\n",aereo.numero);
  if (!pista1) {
    stampevent("Torre: ");
    printf("Autorizzazione al decollo Aereo numero = %d, da pista1\n",aereo.numero);
    pista1 = true;
    kill(aereo.id,SIGALRM);
  }
  else if (!pista2) {
    stampevent("Torre: ");
    printf("Autorizzazione al decollo Aereo numero = %d, da pista2\n",aereo.numero);
    pista2 = true;
  }
  else {
    //no disponibilità quindi wait?
  }

  close(fd);
  return 0;
}
