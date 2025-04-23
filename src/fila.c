// TODO: verificar se essa forma possui justiça

#include "fila.h"
#include <log.h>

fila_t *fila_init(int tamanho_total) {
  fila_t *ret = malloc(sizeof(fila_t));
  ret->buffer = calloc(tamanho_total, sizeof(size_t));
  ret->inicio = 0;
  ret->fim = 0;
  ret->tamanho_total = tamanho_total;
  ret->tamanho = 0;
  pthread_mutex_init(&(ret->lock), NULL);

  return ret;
}

void fila_destroy(fila_t *fila) {
  free(fila->buffer);
  free(fila);
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

size_t fila_get_tamanho(fila_t *fila) { return fila->tamanho; }
