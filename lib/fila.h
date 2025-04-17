#ifndef FILA_H

// WARNING: não é a melhor implementação de fila, mas vai servir para o nosso
// caso
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

// Esta é uma simples implementação de fila de inteiros (size_t)
// Aqui escolhemos esse tipo de inteiro pois estamos guardando o ID de cada
// elfo. Esta é a forma mais correta de representar esse tipo de valor
typedef struct {
  size_t *buffer;
  size_t inicio;
  size_t fim;
  size_t tamanho;
  pthread_mutex_t lock;
} fila_t;

fila_t fila_init(int tamanho);

// Adiciona um elemento no fim da lista (se o elemento anterior não tiver sido
// retirado, ele será sobrescrito
void fila_append(fila_t *fila, uint32_t val);

// Remove um elemento no começo da fila e o retorna
size_t fila_pop(fila_t *fila, int val);

#endif // !FILA_H
