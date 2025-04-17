// TODO: finalizar isso aqui, tendo em vista as threads

#include "fila.h"


fila_t fila_init(int tamanho) {
  fila_t ret = {.buffer = malloc(tamanho * sizeof(int)),
                .inicio = 0,
                .fim = 1,
                .tamanho = tamanho};
  return ret;
}

void fila_append(fila_t *fila, uint32_t val) {
  fila->buffer[fila->fim] = val;
  fila->fim = (fila->fim+1)%fila->tamanho;
}

size_t fila_pop(fila_t *fila, int val) {
  uint32_t ret = fila->buffer[fila->inicio];
  fila->inicio = (fila->inicio+1)%fila->tamanho;
  return ret;
}
