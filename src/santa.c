#include "santa.h"
#include "elfo.h"
#include "log.h"
#include "reindeer.h"
#include "teatro.h"

pthread_t santa_thread;

sem_t semaforo_acordar_santa; // Quando semáforo for exatamente quatro, ele
                              // acordará o Santa e os três elfos serão ajudados

void *santa(void *args) {
  while (true) {
    if(elfos_precisando_de_ajuda < 3){
      print_red("Santa esta dormindo\n");
      santa_set_status(0);
      sem_wait(&semaforo_acordar_santa); // Acorda o Santa
      santa_set_status(1);
      print_red("Santa acordou\n");
    }
    pthread_mutex_lock(&elfos_lock);
    pthread_mutex_lock(&reindeerMutex);
    if (reindeer_count == NUM_OF_REINDEERS) {
      prepareSleigh();

      // Como o Santa irá sair. Devemos finalizar as threads dos elfos.
      // TODO:
      // Como algumas estão esperando um semáforo, eu vou apenas cancelar todas
      // as threads, se tiverem alguma ideia melhor (mais clean), podem mudar
      elfos_kill();
      pthread_mutex_unlock(&reindeerMutex);
      pthread_mutex_unlock(&elfos_lock);
      break;
    } else if (elfos_precisando_de_ajuda == 3) {
      helpElves();
    }

    pthread_mutex_unlock(&reindeerMutex);
    pthread_mutex_unlock(&elfos_lock);
  }
  usleep(USLEEP_DELAY_FIM);
  return NULL;
}

void santa_init() {
  pthread_create(&santa_thread, NULL, santa, NULL);

  pthread_join(santa_thread, NULL);
}

void helpElves() {
  for (int i = 0; i < 3; i++)
    sem_post(&semaforo_elfos_podem_ser_ajudados);
  print_red("Santa esta ajudando os elfos\n");
  santa_chama_elfos();
  usleep(USLEEP_SANTA_AJUDA);

  print_red("Santa terminou de ajudar esses elfos\n");

  elfos_precisando_de_ajuda = 0;
  for (int i = 0; i < 3; i++)
    sem_post(&semaforo_ajuda_finalizada);
  santa_libera_elfos();
}

void prepareSleigh() {
  print_red("Santa esta preparando o treno\n");
  treno_status = 1;
  usleep(USLEEP_PREPARACAO_TRENO);
  print_red("Santa terminou de preparar o treno\n");
  santa_termina_preparacao();
  for (int i = 0; i < NUM_OF_REINDEERS; i++)
    sem_post(&reindeerSem);
  reindeer_count = 0;
}

void prepare_santa() {
  sem_init(&semaforo_acordar_santa, 0, 0);
}