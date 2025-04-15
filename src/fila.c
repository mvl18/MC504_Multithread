// TODO: finalizar isso aqui, tendo em vista as threads

// Esta é uma simples implementação de fila de inteiros
#include <stdlib.h>

typedef struct {
  int *buffer;
  int inicio;
  int fim;
  int tamanho;
} fila_t;

fila_t fila_init(int tamanho) {
  fila_t ret = {.buffer = malloc(tamanho * sizeof(int)),
              .inicio = 0,
              .fim = 1,
              .tamanho = tamanho};
  return ret;
}

void fila_append(fila_t fila, int val) {
}
