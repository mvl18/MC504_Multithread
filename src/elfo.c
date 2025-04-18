// Arquivo que contém todas as funções relacionadas aos elfos

#include "elfo.h"

// O Elfo pode estar em 3 condições
//  1. Trabalhando
//  2. Esperando para ser ajudado
//  3. Sendo ajudado
void *elfo(void *args) {
  size_t id = (size_t)args;
  while (true) {
    printf("O elfo %d está trabalhando", (int)id);
    sleep(rand() % 5);

    // sortear se o elfo está com problema
    if (random() % CHANCE_PROBLEMA == 1) {
      printf("O elfo %d está com problema", (int)id);
      fila_append(&fila_elfos, id);

      while (true) {
        // TODO: Parei aqui, vou voltar depois
        sem_wait(&semaforo_elfos);
        // Verificar se ele é um dos que será atendido pelo papai noel (ou seja,
        // se ele é o primeiro, segundo ou terceiro da fila)
        bool foi_atendido = false;
        for (uint8_t i = 0; i < 3; i++) {
          if (fila_peek(&fila_elfos, i)) {
            getHelp(id, i);
            foi_atendido = true;
          }
        }
        if (foi_atendido == false) {
          sem_post(&semaforo_elfos);
        }
      }
    }
  }
}

// Função executada pelo elfo quando ele está recebendo ajuda
void getHelp(size_t id, uint8_t posicao) {
  printf("o elfo %d está recebendo ajuda", (int)id);
  elfos_atendidos[posicao] = id;
}
