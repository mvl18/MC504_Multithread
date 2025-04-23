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
#include "teatro.h"
// Para compilar use -lSDL2 -lSDL2_image



static pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER;

SDL_Texture* carregar_textura(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path);
    if (!tex) {
        printf("Erro ao carregar imagem %s: %s\n", path, IMG_GetError());
    }
    return tex;
}

int rrandom(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// Inicializa os elfos
Elfo elfos[QUANT_ELFOS] = {0};
int elfos_ajudando[3] = {-1, -1, -1}; // Armazena os índices dos 3 elfos ajudando
int elfo_ajuda = 0;
int santa_status;
int renas_status[NUM_OF_REINDEERS] = {0};
int renas_papai = 0;

void acorda_rena(int id_rena){
    pthread_mutex_lock(&ui_mutex);
    renas_status[id_rena] = 1;
    pthread_mutex_unlock(&ui_mutex);
}
 
void elfo_atualiza(int id_elfo, int novo_status){
    pthread_mutex_lock(&ui_mutex); 
    elfos[id_elfo-1].status = novo_status;
    pthread_mutex_unlock(&ui_mutex);

}
void santa_termina_preparacao(){
    renas_papai = 1;
}
void santa_libera_elfos(){
    pthread_mutex_lock(&ui_mutex);
    for (int j = 0; j < 3; j++)
    {
        elfos_ajudando[j] = -1;
    }
    elfo_ajuda = 0;
    pthread_mutex_unlock(&ui_mutex);
}
void santa_chama_elfos(){
    pthread_mutex_lock(&ui_mutex);
    elfo_ajuda = 1;
    pthread_mutex_unlock(&ui_mutex);
}
void santa_set_status(int status){
    santa_status = status;
}


void* teatro(void * args) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    SDL_Window* window = SDL_CreateWindow("Oficina do Papai Noel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //int WINDOW_WIDTH, WINDOW_HEIGHT;
    //SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

    // Carregar as texturas
    SDL_Texture* tex_elfo = carregar_textura("assets/elfo.png", renderer);
    SDL_Texture* tex_elfoajuda = carregar_textura("assets/elfoajuda.png", renderer);
    SDL_Texture* tex_papai_dorme = carregar_textura("assets/papainoeldorme.png", renderer);
    SDL_Texture* tex_papai_acorda = carregar_textura("assets/papainoel.png", renderer);
    SDL_Texture* tex_rena = carregar_textura("assets/rena.png", renderer);
    SDL_Texture* tex_titulo = carregar_textura("assets/titulo.png", renderer);

    if (!tex_elfo || !tex_elfoajuda || !tex_papai_dorme || !tex_papai_acorda || !tex_rena || !tex_titulo) {
        printf("Erro ao carregar imagens.\n");
        return NULL;
    }

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                quit = 1;
            }
        }

 
        //santa_verifica();

        // Renderização
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Título
        SDL_Rect rect_titulo = {WINDOW_WIDTH / 2 - 350, 20, 700, 100}; // (posição horizontal apartir da lateral, posição vertical apartir do topo, largura do retangulo, altura do retangulo)
        SDL_RenderCopy(renderer, tex_titulo, NULL, &rect_titulo);

        // Papai Noel
        SDL_Rect dst_papai = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 150, 300, 300};
        SDL_RenderCopy(renderer, (santa_status == 1 ? tex_papai_acorda : tex_papai_dorme), NULL, &dst_papai);

        // Elfos

        int ajudando_count = 0;
        
        // Verifica quais elfos estão ajudando e armazena os primeiros 3
        for (int i = 0; i < QUANT_ELFOS; i++) {
            if (elfos[i].status == 2) {
                if (ajudando_count < 3) {
                    elfos_ajudando[ajudando_count] = i; // Salva o índice do elfo ajudando
                }
                ajudando_count++;
            }
        }
        for (int i = 0; i < QUANT_ELFOS; i++)
        {
            SDL_Rect dst_elfo;
            if (elfo_ajuda &&
                (i == elfos_ajudando[0] || i == elfos_ajudando[1] || i == elfos_ajudando[2]))
            {
                dst_elfo.x = WINDOW_WIDTH / 2 - 150;                                                    // Perto do centro da tela
                dst_elfo.y = 200 + (i == elfos_ajudando[0] ? 0 : (i == elfos_ajudando[1] ? 100 : 200)); // Espaçamento vertical
            }
            else
            {
                // Elfos restantes permanecem em suas posições originais
                dst_elfo.x = 50 + 25 * elfos[i].status*elfos[i].status ;
                dst_elfo.y = 100 + i * 100;
            }
            dst_elfo.w = 100;
            dst_elfo.h = 100;
        
            // Renderiza a textura correspondente ao status do elfo
            SDL_RenderCopy(renderer, (elfos[i].status ? tex_elfoajuda : tex_elfo), NULL, &dst_elfo);
        }

        // Renas
        for (int i = 0; i < NUM_OF_REINDEERS; i++) {
            SDL_Rect dst_rena;
            if (renas_status[i])
            {
                if (renas_papai) {
                    dst_rena.x = 450 + (i % 3) * 120;
                    dst_rena.y = 450 + (i / 3) * 120;
                } else {
                    dst_rena.x = WINDOW_WIDTH - 100;
                    dst_rena.y = 50 + i * 100;
                }
                dst_rena.w = 100;
                dst_rena.h = 100;
                SDL_RenderCopy(renderer, tex_rena, NULL, &dst_rena);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    // Cleanup
    SDL_DestroyTexture(tex_elfo);
    SDL_DestroyTexture(tex_elfoajuda);
    SDL_DestroyTexture(tex_papai_dorme);
    SDL_DestroyTexture(tex_papai_acorda);
    SDL_DestroyTexture(tex_rena);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

pthread_t teatro_thread;

void teatro_init() {
  pthread_create(&teatro_thread, NULL, teatro, NULL);

}

void teatro_close(){
  pthread_join(teatro_thread, NULL);
}