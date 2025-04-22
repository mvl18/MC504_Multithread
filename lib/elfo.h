#ifndef __ELFO_H__
#define __ELFO_H__

#include "fila.h"
#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t elfos_lock; // Lock para todos as variáveis relacionadas
                                   // aos elfos que necessitarem

extern sem_t semaforo_elfos; // Este semáforo indica quantos faltam para haver
                             // exatamente três elfos precisando de ajuda

extern sem_t semaforo_tres_elfos; // Este semáforo se torna 1 quando há 3 elfos
                                  // esperando para serem ajudados

// TODO: Implementar no Santa
extern sem_t semaforo_elfos_podem_ser_ajudados; // O Santa torna o valor deste
                                                // semáforo em três para avisar
                                                // cada um dos três elfos que
                                                // eles podem ser ajudados

// TODO: Implementar no Santa
extern sem_t semaforo_ajuda_finalizada; // O Santa torna o valor deste semáforo
                                        // em três para avisar os três elfos que
                                        // terminou de ajudá-los

extern pthread_t threads_elfos[QUANT_ELFOS]; // Contém os IDs de todas as
                                             // threads de elfos

extern int elfos_precisando_de_ajuda; // Indica quantos elfos precisam de ajuda

extern fila_t *fila_elfos;
extern size_t elfos_sendo_ajudados[3];

// Esta função inicializa todas as threads de elfos e todos os semáforos
// relacionados aos elfos
void elfos_init();

// Esta função dá join em todas as threads de elfos
void elfos_close();

// Esta função termina imediatamente todas as threads de elfos
void elfos_kill();

void *elfo(void *args);

void getHelp(int id);

#endif // !__ELFO_H__
