#ifndef __REINDEER_H__
#define __REINDEER_H__


#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t reindeerMutex; // Lock para todos as variáveis relacionadas as renas

extern sem_t reindeerSem; // O Santa torna o valor deste semaforo e 9 para avisar
                          // cada uma das renas que elas podem se engatar ao trenó

extern int reindeer_count; // Numero de renas que voltaram para se engatar ao trenó

extern pthread_t reindeer_threads[NUM_OF_REINDEERS]; // Contém os IDs de todas as
                                                     // threads de renas

void getHitched(int id);

void reindeer_init();

void reindeer_close();

#endif // !__REINDEER_H__