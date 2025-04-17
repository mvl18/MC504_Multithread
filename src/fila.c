// TODO: adicionar mutex

#include "fila.h"

fila_t fila_init(int tamanho) {
  fila_t ret = {
      .buffer = malloc(tamanho * sizeof(int)),
      .inicio = 0,
      .fim = 1,
      .tamanho = tamanho,
      .lock = PTHREAD_MUTEX_INITIALIZER,
  };
  return ret;
}

void fila_append(fila_t *fila, uint32_t val) {

  pthread_mutex_lock(&fila->lock);

  fila->buffer[fila->fim] = val;
  fila->fim = (fila->fim + 1) % fila->tamanho;

  pthread_mutex_unlock(&fila->lock);
}

size_t fila_pop(fila_t *fila, int val) {

  pthread_mutex_lock(&fila->lock);

  uint32_t ret = fila->buffer[fila->inicio];
  fila->inicio = (fila->inicio + 1) % fila->tamanho;

  pthread_mutex_unlock(&fila->lock);

  return ret;
}
