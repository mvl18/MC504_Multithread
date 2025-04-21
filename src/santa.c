#include "santa.h"
#include "elfo.h"
#include "reindeer.h"
#include "log.h"

pthread_t santa_thread;

sem_t semaforo_acordar_santa; // Quando semáforo for exatamente quatro, ele
                              // acordará o Santa e os três elfos serão ajudados

void *santa(void *args) {
  while (true) {
    print_red("Santa está dormindo\n");

    sem_wait(&semaforo_acordar_santa); // Acorda o Santa

    print_red("Santa acordou\n");
    
    pthread_mutex_lock(&elfos_lock);
    pthread_mutex_lock(&reinderMutex);
    if (reindeer_count == NUM_OF_REINDEERS) {
      prepareSleigh();
    }
    else if (elfos_precisando_de_ajuda == 3) {
      helpElves();
    }

    pthread_mutex_unlock(&reinderMutex);
    pthread_mutex_unlock(&elfos_lock);
  
  }

  return NULL;
}

void santa_init() {
  pthread_create(&santa_thread, NULL, santa, NULL);

  pthread_join(santa_thread, NULL);
}

void helpElves(){
  sleep(rand()%5);

  print_red("Santa terminou de ajudar esses elfos\n");

  for (int i = 0; i < 3; i++)
    sem_post(&semaforo_ajuda_finalizada);
}

void prepareSleigh(){
  print_red("Santa está preparando o trenó\n");
  sleep(rand()%5);
  print_red("Santa terminou de preparar o trenó\n");
  for (int i = 0; i < NUM_OF_REINDEERS; i++)
                sem_post(&reindeerSem);
  reindeer_count = 0;
}