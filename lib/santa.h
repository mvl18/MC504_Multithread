#ifndef __SANTA_H__
#define __SANTA_H__

#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_t santa_thread;

extern sem_t semaforo_acordar_santa; // Quando semáforo for exatamente quatro, ele
                              // acordará o Santa e os três elfos serão ajudados

void santa_init();

void helpElves();

void prepareSleigh();

void prepare_santa();

#endif // !__SANTA_H__
