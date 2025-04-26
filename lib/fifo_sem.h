#ifndef __FIFO_SEM_H
#define __FIFO_SEM_H

#include <semaphore.h>  // Para sem_t
#include <fila.h>

#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Estrutura do semáforo FIFO
typedef struct {
    sem_t semaforo;     // Semáforo interno
    fila_t *fila;       // Fila de espera (FIFO)
    int counter;        // Contador interno (se necessário)
} fifo_sem_t;

// Inicializa o semáforo FIFO
void fifo_sem_init(fifo_sem_t *sem, int start);

// Libera o semáforo (versão simplificada)
void fifo_sem_post(fifo_sem_t *sem);

// Solicita acesso ao recurso com ID (ordem FIFO)
void fifo_sem_wait(fifo_sem_t *sem, int id);

// Destroi o semáforo e libera recursos
void fifo_sem_destroy(fifo_sem_t *sem);

#endif // __FIFO_SEM_H