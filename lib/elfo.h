#ifndef ELFO_H
#define ELFO_H

#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t semaforo_elfos; // Este semáforo indica quantos faltam para haver
                      // exatamente três elfos precisando de ajuda

sem_t semaforo_fila_cheia;

// TODO: Implementar no Santa
sem_t semaforo_ajuda_finalizada; // O Santa torna o valor deste semáforo em três
                                 // para avisar os três elfos que terminou de
                                 // ajudá-los

pthread_t threads_elfos[QUANT_ELFOS]; // Contém os IDs de todas as
                                      // threads de elfos

// Contém os IDs dos elfos que precisam de ajuda
//
// NOTE: É inicializado em zero pois os IDs dos elfos começam a contar a partir
// de 1
size_t elfos_precisando_de_ajuda[3] = {0};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Esta função inicializa todas as threads de elfos e todos os semáforos
// relacionados aos elfos
void elfos_init();

// Esta função dá join em todas as threads de elfos
void elfos_close();

void *elfo(void *args);

void getHelp(size_t id);

#endif // !ELFO_H
