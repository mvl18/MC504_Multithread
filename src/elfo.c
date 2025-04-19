// Arquivo que contém todas as funções relacionadas aos elfos

#include "elfo.h"

void elfos_init() {
  sem_init(&semaforo_elfos, NULL, 3); // O semáforo é inicializado com três
                                      // pois três elfos devem ser atendidos
                                      // ao mesmo tempo
  sem_init(&semaforo_ajuda_finalizada, NULL, 0);

  // Criar Threads de Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_create(&threads_elfos[i], NULL, elfo, (void *)i + 1);
  }
}

void elfos_close() {
  // Unir threads dos Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_join(threads_elfos[i], NULL);
  }
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

      // Este elfo que acabou de ficar com problema, deve esperar até que o
      // Santa não esteja ajudando ninguém para pedir ajuda
      sem_wait(&semaforo_elfos);

      printf("O elfo %d está com problema", (int)id);

      // TODO: Fazer o seguinte somente quando houverem exatamente 3 elfos
      // esperando para serem ajudados
      // talvez usar barreira seja uma boa ideia
      getHelp(id);
    }
  }
}

// Função executada pelo elfo quando ele está recebendo ajuda
void getHelp(size_t id) {
  printf("o elfo %d está recebendo ajuda", (int)id);
  sem_wait(&semaforo_ajuda_finalizada);
  printf("O elfo %d acabou de ser ajudado e voltará a trabalhar", (int)id);
}
