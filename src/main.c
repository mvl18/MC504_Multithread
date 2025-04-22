#include "elfo.h"
#include "log.h"
#include "reindeer.h"
#include "santa.h"
#include <pthread.h>

int main() {
  elfos_init();
  reindeer_init();
  santa_init();
  printf("1\n");
  elfos_close();
  printf("2\n");
  reindeer_close();
  printf("3\n");

  print_green("O Programa finalizou\n");
  return 0;
}
