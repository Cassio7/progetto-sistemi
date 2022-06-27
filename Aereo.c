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

int rangerand(int min, int max){
  static int bSeed = 0;
  if(!bSeed) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_usec % 1000);
    bSeed = 1;
  }
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
  printf("Inizio preparazione Aereo numero ) %d, durata = %d\n",aereo.numero,r);
  sleep(r);
}
