#ifndef ELFO_H
#define ELFO_H

#include "fila.h"
#include "globals.h"
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

fila_t *fila_elfos;

sem_t semaforo_elfos; // Este semáforo indica quantos faltam para haver
                      // exatamente três elfos precisando de ajuda

// TODO: Implementar no Santa
sem_t semaforo_ajuda_finalizada; // O Santa torna o valor deste semáforo em três
                                 // para avisar os três elfos que terminou de
                                 // ajudá-los

pthread_t threads_elfos[QUANT_ELFOS]; // Contém os IDs de todas as threads de elfos

// Esta função inicializa todas as threads de elfos e todos os semáforos
// relacionados aos elfos
void elfos_init();

// Esta função dá join em todas as threads de elfos
void elfos_close();

void *elfo(void *args);

void getHelp(size_t id, uint8_t posicao);

#endif // !ELFO_H
