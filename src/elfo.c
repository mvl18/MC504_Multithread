// Todos os comentários sobre as variáveis e as funções estão em elfo.h
#include "elfo.h"
#include "log.h"
#include "santa.h"
#include "teatro.h"
#include "fifo_sem.h"
pthread_mutex_t elfos_lock = PTHREAD_MUTEX_INITIALIZER;
fifo_sem_t semaforo_elfos;
sem_t semaforo_tres_elfos;
sem_t semaforo_elfos_podem_ser_ajudados;
sem_t semaforo_ajuda_finalizada;
pthread_t threads_elfos[QUANT_ELFOS];
int elfos_precisando_de_ajuda = 0;


void elfos_init() {
  // O semáforo é inicializado com três pois três elfos devem ser atendidos ao
  // mesmo tempo
  fifo_sem_init(&semaforo_elfos, 3);

  sem_init(&semaforo_tres_elfos, 0, 0);

  sem_init(&semaforo_elfos_podem_ser_ajudados, 0, 0);

  // Este semáforo deve se tornar 3 para avisar os três elfos de que a ajuda
  // foi finalizada
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
  fifo_sem_destroy(&semaforo_elfos);
  pthread_mutex_destroy(&elfos_lock);
}

void elfos_kill() {
  // Cancelar threads dos Elfos
  for (size_t i = 0; i < QUANT_ELFOS; i++) {
    pthread_cancel(threads_elfos[i]);
  }
  pthread_mutex_destroy(&elfos_lock);
}

void *elfo(void *args) {
  size_t id = (size_t)args;
  while (true) {
    print_green("O elfo %d esta trabalhando\n", (int)id);
    usleep(USLEEP_ELFO_TRABALHANDO);

    // sortear se o elfo está com problema
    if (random() % CHANCE_PROBLEMA == 0) {
      print_green("O elfo %d esta com problema\n", (int)id);
      print_green("O elfo %d vai pra fila\n", (int)id);
      elfo_atualiza(id, 1);
      fifo_sem_wait(&semaforo_elfos, id);
      print_green("O elfo %d esta no grupo que sera atendido\n", (int)id);
      elfo_atualiza(id, 2);
      pthread_mutex_lock(&elfos_lock);
      elfos_precisando_de_ajuda++;
      // Se houverem exatamente três elfos precisando de ajuda, permitimos os
      // elfos irem acordar o Santa
      if (elfos_precisando_de_ajuda == 3) {
        sem_post(&semaforo_acordar_santa);
      } else if (elfos_precisando_de_ajuda > 3) {
        fprintf(stderr,
                "Ha mais de 3 elfos precisando de ajuda ao mesmo tempo\n");
      }

      pthread_mutex_unlock(&elfos_lock);

      sem_wait(&semaforo_elfos_podem_ser_ajudados);
      getHelp(id);
    }
    elfo_atualiza(id, 0);
  }
}

// Função executada pelo elfo quando ele está recebendo ajuda
void getHelp(int id) {
  print_green("O elfo %d esta recebendo ajuda\n", (int)id);
  sem_wait(&semaforo_ajuda_finalizada);

  fifo_sem_post(&semaforo_elfos); 
  print_green("O elfo %d acabou de ser ajudado e voltara a trabalhar\n",
              (int)id);
}
