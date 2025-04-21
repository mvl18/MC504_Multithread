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

extern pthread_mutex_t reinderMutex;

extern sem_t reindeerSem;

extern int reindeer_count;

extern pthread_t reindeer_threads[NUM_OF_REINDEERS];

void getHitched(int id);

void reindeer_init();

void reindeer_close();

#endif // !__REINDEER_H__