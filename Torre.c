#include "def.h"


bool pista1=false,pista2=false;

int main() {
  stampevent("Torre: ");
  printf("Avvio del processo Torre\n");
  struct aereo aereo;
  int fd = open("/tmp/myfifo", O_RDONLY);
  if(read(fd, &aereo, sizeof(aereo)) == -1) {
    perror("Parent: Errore in read");
    return 1;
  }
  stampevent("Torre :");
  printf("Richiesta di decollo da Aereo numero = %d\n",aereo.numero);
  return 0;
}
