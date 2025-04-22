#include "elfo.h"
#include "log.h"
#include "reindeer.h"
#include "santa.h"
#include <pthread.h>

int main() {
  print_blue("Iniciando o programa...\n");

  prepare_santa();
  elfos_init();
  reindeer_init();
  santa_init();
  elfos_close();
  reindeer_close();

  print_blue("O Programa finalizou!\n");
  return 0;
}
