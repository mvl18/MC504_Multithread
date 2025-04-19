#include "elfo.h"
#include <globals.h>
#include <pthread.h>

void prepareSleigh();
void getHitched();
void helpElves();

int main() {
  // Inicializando a fila de elfos
  fila_elfos = fila_init(QUANT_ELFOS);
  pthread_t threads_elfos[QUANT_ELFOS];

  // Criar Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_create(&threads_elfos[i], NULL, elfo, (void *)i);
  }

  // Unir threads dos Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_join(threads_elfos[i], NULL);
  }
}
