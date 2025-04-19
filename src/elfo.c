// NOTE: Estou parando de resolver utilizando fila porque não condiz com as
// condições do problema.

// Arquivo que contém todas as funções relacionadas aos elfos

#include "elfo.h"

void elfos_init() {
  sem_init(&semaforo_elfos, NULL, 3); // O semáforo é inicializado com três
                                      // pois três elfos devem ser atendidos
                                      // ao mesmo tempo
  sem_init(&semaforo_elfos, NULL, 0);

  // Inicializando a fila de elfos
  fila_elfos = fila_init(QUANT_ELFOS);

  // Criar Threads de Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_create(&threads_elfos[i], NULL, elfo, (void *)i);
  }
}

void elfos_close() {
  // Unir threads dos Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_join(threads_elfos[i], NULL);
  }

  // Destruindo a fila
  fila_destroy(fila_elfos);
}

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
      fila_append(fila_elfos, id);

      // Se houver três ou mais elfos na fila, incrementar o semáforo de elfos
      // em três
      if (fila_get_tamanho(fila_elfos) >= 3) { // BUG: Isso aqui tá errado
        sem_post(&semaforo_elfos);
        sem_post(&semaforo_elfos);
        sem_post(&semaforo_elfos);
      }

      while (true) {

        // Esse semáforo deve ser liberado pelo terceiro elfo quando entrar na
        // fila. Apenas 3 elfos fazem a verificação por vez se ele está na
        // frente da fila.
        sem_wait(&semaforo_elfos);

        // Verificar se ele é um dos que será atendido pelo papai noel (ou seja,
        // se ele é o primeiro, segundo ou terceiro da fila)
        bool foi_atendido = false;
        for (uint8_t i = 0; i < 3; i++) {
          if (fila_peek(fila_elfos, i)) {
            getHelp(id, i);
            foi_atendido = true;
          }
        }

        // Se ele não é um dos três primeiros da fila, ele libera o semáforo
        // para que outro elfo verifique
        if (foi_atendido == false) {
          sem_post(&semaforo_elfos);

          // Dá um sleep de 1 segundo pra dar tempo de outras thread fazerem
          // suas verificações
          sleep(1);
        }
      }
    }
  }
}

// Função executada pelo elfo quando ele está recebendo ajuda
void getHelp(size_t id, uint8_t posicao) {
  printf("o elfo %d está recebendo ajuda", (int)id);
  sem_wait(&semaforo_ajuda_finalizada);
  fila_pop(fila_elfos); // "Se retirar da fila"
                        // NOTE:
                        // Na verdade o que ocorre é que ele retirará po
                        // primeiro da fila, mas como há três elfos sendo
                        // ajudados, os três se retirarão mutualmente da fila,
                        // fazendo com que o resultado final seja o mesmo de se
                        // ele tivesse se retirado
}
