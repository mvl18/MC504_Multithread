// Todos os comentários sobre as variáveis e as funções estão em elfo.h
#include "elfo.h"
#include "log.h"
#include "santa.h"

pthread_mutex_t elfos_lock = PTHREAD_MUTEX_INITIALIZER;

sem_t semaforo_elfos;

sem_t semaforo_ajuda_finalizada;

pthread_t threads_elfos[QUANT_ELFOS];

int elfos_precisando_de_ajuda = 0;

void elfos_init() {
  sem_init(&semaforo_acordar_santa, 0,
           0); // Isso não era pra tá aqui , mas esse semaforo precisa existir
               // antes das threads
  sem_init(&semaforo_elfos, 0, 3); // O semáforo é inicializado com três
                                   // pois três elfos devem ser atendidos
                                   // ao mesmo tempo
  sem_init(&semaforo_ajuda_finalizada, 0, 0);

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
  pthread_mutex_destroy(&elfos_lock);
}

void *elfo(void *args) {
  size_t id = (size_t)args;
  while (true) {
    print_green("O elfo %d está trabalhando\n", (int)id);
    sleep(rand() % 5);

    // sortear se o elfo está com problema
    if (random() % CHANCE_PROBLEMA == 1) {

      // Este elfo que acabou de ficar com problema, deve esperar até que o
      // Santa não esteja ajudando ninguém para pedir ajuda
      sem_wait(&semaforo_elfos);
      print_green("O elfo %d está com problema\n", (int)id);

      pthread_mutex_lock(&elfos_lock);

      elfos_precisando_de_ajuda++;

      // Se houverem exatamente três elfos precisando de ajuda, permitimos os
      // elfos irem acordar o Santa
      if (elfos_precisando_de_ajuda == 3) {
        sem_post(&semaforo_acordar_santa);
      } else if (elfos_precisando_de_ajuda > 3) {
        fprintf(stderr,
                "Há mais de 3 elfos precisando de ajuda ao mesmo tempo\n");
      }

      pthread_mutex_unlock(&elfos_lock);

      getHelp(id);
    }
  }
}

// Função executada pelo elfo quando ele está recebendo ajuda
void getHelp(int id) {
  print_green("o elfo %d está recebendo ajuda\n", (int)id);
  sem_wait(&semaforo_ajuda_finalizada);
  sem_post(&semaforo_elfos);
  print_green("O elfo %d acabou de ser ajudado e voltará a trabalhar\n",
              (int)id);
}
