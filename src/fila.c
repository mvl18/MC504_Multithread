// TODO: verificar se essa forma possui justiça

#include "fila.h"

fila_t fila_init(int tamanho_total) {
  fila_t ret = {
      .buffer = malloc(tamanho_total * sizeof(int)),
      .inicio = 0,
      .fim = 1,
      .tamanho_total = tamanho_total,
      .tamanho = 0,
      .lock = PTHREAD_MUTEX_INITIALIZER,
  };
  return ret;
}

void fila_append(fila_t *fila, size_t val) {

  pthread_mutex_lock(&fila->lock);

  fila->buffer[fila->fim] = val;
  fila->fim = (fila->fim + 1) % fila->tamanho_total;
  fila->tamanho++;

  pthread_mutex_unlock(&fila->lock);
}

size_t fila_pop(fila_t *fila) {

  pthread_mutex_lock(&fila->lock);

  uint32_t ret = fila->buffer[fila->inicio];
  fila->inicio = (fila->inicio + 1) % fila->tamanho_total;
  fila->tamanho--;

  pthread_mutex_unlock(&fila->lock);

  return ret;
}

size_t fila_peek(fila_t *fila, size_t posicao) {
  // Se estamos querendo um valor além da quantidade de itens na fila,
  // retornamos -1 (isto é, 0xffffffff)
  if (posicao >= fila->tamanho) {
    return -1;
  }
  return fila->buffer[(fila->inicio + posicao) % fila->tamanho_total];
}
