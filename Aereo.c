#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

struct aereo {
  int id;
  int numero;
};

void child(pid_t pid,int i){
  struct aereo aereo = {pid,i};
  stampevent("Aereo: ");
  printf("Aereo numero = %d, id = %d\n",aereo.numero,aereo.id );
}
