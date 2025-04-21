#include <globals.h>
#include <pthread.h>
#include "elfo.h"
#include "santa.h"
#include "reindeer.h"

int main() {
  elfos_init();
  reindeer_init();
  santa_init();
  elfos_close();
  reindeer_close();
  return 0;
}
