#include "elfo.h"
#include "log.h"
#include "reindeer.h"
#include "santa.h"
#include <pthread.h>

int main() {
  elfos_init();
  reindeer_init();
  santa_init();
  elfos_close();
  reindeer_close();

  print_blue("O Programa finalizou!\n");
  return 0;
}
