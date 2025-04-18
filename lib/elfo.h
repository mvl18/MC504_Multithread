#ifndef ELFO_H
#define ELFO_H

#include "fila.h"
#include "globals.h"
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

fila_t fila_elfos;

// TODO: Fazer o papai Noel incrementar o semáforo dos elfos 3 vezes para atendê-los
sem_t semaforo_elfos; // Este semáfor se torna 3 quando o papai noel está
                      // acordado para atender 3 elfos. E se torna zero quando o
                      // papai noel não está disponível para atender os elfos

size_t elfos_atendidos[3]; // Lista dos elfos sendo atendidos

void *elfo(void *args);

void getHelp(size_t id, uint8_t posicao);

#endif // !ELFO_H
