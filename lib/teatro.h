#ifndef __TEATRO_H__
#define __TEATRO_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "globals.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "elfo.h"
#include "log.h"

// Structures
typedef struct {
    int status; // 0 = trabalhando, 1 = na fila, 2 = ajudado, 3 = finalizado
} Elfo;

typedef struct {
    int x, y;
} Posicao;

// Global variables
extern Elfo elfos[QUANT_ELFOS];
extern int elfos_ajudando[3];
extern int elfo_ajuda;
extern int santa_status;
extern int renas_status[NUM_OF_REINDEERS];
extern int renas_papai;

// Function declarations
SDL_Texture* carregar_textura(const char* path, SDL_Renderer* renderer);
int rrandom(int min, int max);

// Rena functions
void acorda_rena(int id_rena);

// Elfo functions
void elfo_atualiza(int id_elfo, int novo_status);
void ajuda_elfo(int id_elfo);

// Santa functions
void santa_libera_elfos(void);
void santa_chama_elfos(void);
void santa_verifica(void);
void santa_termina_preparacao();
void santa_set_status(int status);

// Main game loop
int main(void);

void teatro_init();
void teatro_close();
#endif /* __TEATRO_H__ */