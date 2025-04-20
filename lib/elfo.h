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

// Lock para todos as variáveis relacionadas aos elfos que necessitarem
pthread_mutex_t elfos_lock = PTHREAD_MUTEX_INITIALIZER;

sem_t semaforo_elfos; // Este semáforo indica quantos faltam para haver
                      // exatamente três elfos precisando de ajuda

sem_t semaforo_acordar_santa; // Quando semáforo for exatamente quatro, ele
                              // acordará o Santa e os três elfos serão ajudados

// TODO: Implementar no Santa
sem_t semaforo_ajuda_finalizada; // O Santa torna o valor deste semáforo em três
                                 // para avisar os três elfos que terminou de
                                 // ajudá-los

pthread_t threads_elfos[QUANT_ELFOS]; // Contém os IDs de todas as
                                      // threads de elfos

// Indica quantos elfos precisam de ajuda
size_t elfos_precisando_de_ajuda = 0;

// Esta função inicializa todas as threads de elfos e todos os semáforos
// relacionados aos elfos
void elfos_init();

// Esta função dá join em todas as threads de elfos
void elfos_close();

void *elfo(void *args);

void getHelp(size_t id);

#endif // !ELFO_H
