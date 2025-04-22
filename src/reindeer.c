#include "reindeer.h"
#include "log.h"
#include "santa.h"

pthread_mutex_t reindeerMutex = PTHREAD_MUTEX_INITIALIZER;

sem_t reindeerSem;

int reindeer_count = 0;

pthread_t reindeer_threads[NUM_OF_REINDEERS];

void getHitched(int id) { print_yellow("Rena %d: Engatada ao trenó!\n", id); }

void *reindeer(void *args) {
  size_t id = (size_t)args;

  sleep(rand() % 10 + 5); // Férias
  pthread_mutex_lock(&reindeerMutex);
  reindeer_count++;
  print_yellow("Rena %li voltou. Total: %d\n", id, reindeer_count);
  if (reindeer_count == NUM_OF_REINDEERS)
    sem_post(&semaforo_acordar_santa);
  pthread_mutex_unlock(&reindeerMutex);

  sem_wait(&reindeerSem);
  getHitched(id);

  return NULL;
}

void reindeer_init() {
  sem_init(&reindeerSem, 0, 0);

  for (size_t i = 0; i < NUM_OF_REINDEERS; i++) {
    pthread_create(&reindeer_threads[i], NULL, reindeer, (void *)(i + 1));
  }
}

void reindeer_close() {
  for (int i = 0; i < NUM_OF_REINDEERS; i++) {
    pthread_join(reindeer_threads[i], NULL);
  }
  pthread_mutex_destroy(&reindeerMutex);
}
