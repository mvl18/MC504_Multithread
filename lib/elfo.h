#ifndef __ELFO_H__
#define __ELFO_H__

#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Lock para todos as variáveis relacionadas aos elfos que necessitarem
extern pthread_mutex_t elfos_lock;

extern sem_t semaforo_elfos; // Este semáforo indica quantos faltam para haver
                      // exatamente três elfos precisando de ajuda

// TODO: Implementar no Santa
extern sem_t semaforo_ajuda_finalizada; // O Santa torna o valor deste semáforo em três
                                 // para avisar os três elfos que terminou de
                                 // ajudá-los

extern pthread_t threads_elfos[QUANT_ELFOS]; // Contém os IDs de todas as
                                      // threads de elfos

// Indica quantos elfos precisam de ajuda
extern int elfos_precisando_de_ajuda;

// Esta função inicializa todas as threads de elfos e todos os semáforos
// relacionados aos elfos
void elfos_init();

// Esta função dá join em todas as threads de elfos
void elfos_close();

void *elfo(void *args);

void getHelp(int id);

#endif // !__ELFO_H__
