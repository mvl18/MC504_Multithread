#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

#include "elfoajuda_png.h"
#include "elfo_png.h"
#include "papainoeldorme_png.h"
#include "papainoel_png.h"
#include "rena_png.h"
#include "titulo_png.h"
#include "treno_png.h"

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
int treno_status = 0;

void acorda_rena(int id_rena){
    pthread_mutex_lock(&ui_mutex);
    renas_status[id_rena - 1] = 1;
    pthread_mutex_unlock(&ui_mutex);
}
 
void elfo_atualiza(int id_elfo, int novo_status){
    pthread_mutex_lock(&ui_mutex); 
    elfos[id_elfo-1].status = novo_status;
    pthread_mutex_unlock(&ui_mutex);

}
void santa_termina_preparacao(){
    pthread_mutex_lock(&ui_mutex);
    renas_papai = 1;
    pthread_mutex_unlock(&ui_mutex);
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
    pthread_mutex_lock(&ui_mutex);
    santa_status = status;
    pthread_mutex_unlock(&ui_mutex);

}

// Criação de buffer circular para armazenar as mensagens de log e adicionar no SDL
char log_buffer[LOG_LINES][LOG_LINE_LENGTH];
int log_index = 0;

void add_to_log(const char *message) {
    pthread_mutex_lock(&ui_mutex);
    snprintf(log_buffer[log_index], LOG_LINE_LENGTH, "%s", message);
    log_index = (log_index + 1) % LOG_LINES; // Atualiza o índice circularmente
    pthread_mutex_unlock(&ui_mutex);
}

const char * get_from_log(int i){
    int log_line = (log_index + i) % LOG_LINES; // Acessa as linhas do buffer circular
    const char *value = log_buffer[log_line];
    return value;
}

void* teatro(void * args) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    SDL_Window* window = SDL_CreateWindow("Oficina do Papai Noel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // Inicialização do TTF
    if (TTF_Init() == -1) {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        return 0;
    }

    TTF_Font *font = TTF_OpenFont("assets/monica.ttf", 24);
    if (!font) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        return 0;
    }

    //int WINDOW_WIDTH, WINDOW_HEIGHT;
    //SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
    // Elfo ajuda
    SDL_Surface* loaded_surface_elfoajuda = IMG_Load_RW(SDL_RWFromMem(elfoajuda_png, elfoajuda_png_len), 1);
    if (!loaded_surface_elfoajuda) {
        printf("Erro ao carregar a imagem do elfo_ajuda: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_elfoajuda = SDL_CreateTextureFromSurface(renderer, loaded_surface_elfoajuda);
    if (!tex_elfoajuda) {
        printf("Erro ao criar textura do elfo_ajuda: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_elfoajuda);
        return 0;
    }

    // Elfo
    SDL_Surface* loaded_surface_elfo = IMG_Load_RW(SDL_RWFromMem(elfo_png, elfo_png_len), 1);
    if (!loaded_surface_elfo) {
        printf("Erro ao carregar a imagem do elfo: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_elfo = SDL_CreateTextureFromSurface(renderer, loaded_surface_elfo);
    if (!tex_elfo) {
        printf("Erro ao criar textura do elfo: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_elfo);
        return 0;
    }

    // Papai noel dorme
    SDL_Surface* loaded_surface_papai_dorme = IMG_Load_RW(SDL_RWFromMem(papainoeldorme_png, papainoeldorme_png_len), 1);
    if (!loaded_surface_papai_dorme) {
        printf("Erro ao carregar a imagem do papai_dorme: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_papai_dorme = SDL_CreateTextureFromSurface(renderer, loaded_surface_papai_dorme);
    if (!tex_papai_dorme) {
        printf("Erro ao criar textura do papai_dorme: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_papai_dorme);
        return 0;
    }

    // papai noel acorda
    SDL_Surface* loaded_surface_papai_acorda = IMG_Load_RW(SDL_RWFromMem(papainoel_png, papainoel_png_len), 1);
    if (!loaded_surface_papai_acorda) {
        printf("Erro ao carregar a imagem do papai_acorda: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_papai_acorda = SDL_CreateTextureFromSurface(renderer, loaded_surface_papai_acorda);
    if (!tex_papai_acorda) {
        printf("Erro ao criar textura do papai_acorda: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_papai_acorda);
        return 0;
    }

    // titulo
    SDL_Surface* loaded_surface_titulo = IMG_Load_RW(SDL_RWFromMem(titulo_png, titulo_png_len), 1);
    if (!loaded_surface_titulo) {
        printf("Erro ao carregar a imagem do titulo: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_titulo = SDL_CreateTextureFromSurface(renderer, loaded_surface_titulo);
    if (!tex_titulo) {
        printf("Erro ao criar textura do titulo: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_titulo);
        return 0;
    }

    // rena
    SDL_Surface* loaded_surface_rena = IMG_Load_RW(SDL_RWFromMem(rena_png, rena_png_len), 1);
    if (!loaded_surface_rena) {
        printf("Erro ao carregar a imagem da rena: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_rena = SDL_CreateTextureFromSurface(renderer, loaded_surface_rena);
    if (!tex_rena) {
        printf("Erro ao criar textura da rena: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_rena);
        return 0;
    }

    // treno
    SDL_Surface* loaded_surface_treno = IMG_Load_RW(SDL_RWFromMem(treno_png, treno_png_len), 1);
    if (!loaded_surface_treno) {
        printf("Erro ao carregar a imagem do treno: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Texture* tex_treno = SDL_CreateTextureFromSurface(renderer, loaded_surface_treno);
    if (!tex_treno) {
        printf("Erro ao criar textura do treno: %s\n", SDL_GetError());
        SDL_FreeSurface(loaded_surface_treno);
        return 0;
    }


    // Carregar as texturas
    //SDL_Texture* tex_elfo = carregar_textura("assets/elfo.png", renderer);
    //SDL_Texture* tex_elfoajuda = carregar_textura("assets/elfoajuda.png", renderer);
    //SDL_Texture* tex_papai_dorme = carregar_textura("assets/papainoeldorme.png", renderer);
    //SDL_Texture* tex_papai_acorda = carregar_textura("assets/papainoel.png", renderer);
    //SDL_Texture* tex_rena = carregar_textura("assets/rena.png", renderer);
    //SDL_Texture* tex_titulo = carregar_textura("assets/titulo.png", renderer);

    //if (!tex_elfo || !tex_elfoajuda || !tex_papai_dorme || !tex_papai_acorda || !tex_rena || !tex_titulo) {
    //    printf("Erro ao carregar imagens.\n");
    //    return NULL;
    //}

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
        pthread_mutex_lock(&ui_mutex);
        // Renderizar o log na parte inferior da tela
        for (int i = 0; i < LOG_LINES; i++) {

            char clean_text[LOG_LINE_LENGTH];
            remove_ansi_sequences(get_from_log(i), clean_text); // Remove sequências ANSI
            remove_newline(clean_text);
            
            // Determinar a cor com base no prefixo
            SDL_Color textColor = {255, 255, 255, 255}; // Cor padrão (branca)
            if (strstr(clean_text, "Santa")) {
                textColor = (SDL_Color){255, 0, 0, 255}; // Vermelho
            } else if (strstr(clean_text, "O")) {
                textColor = (SDL_Color){0, 255, 0, 255}; // Verde
            } else if (strstr(clean_text, "Rena")) {
                textColor = (SDL_Color){255, 255, 0, 255}; // Amarelo
            } else if (strstr(clean_text, "O Programa")) {
                textColor = (SDL_Color){0, 0, 255, 255}; // Azul
            }

            SDL_Surface *textSurface = TTF_RenderText_Blended(font, clean_text, textColor);
            if (textSurface) {
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = {WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT - 280 + i * 30, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }

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
                (i == elfos_ajudando[0] || i == elfos_ajudando[1] || i == elfos_ajudando[2])) {
                // Posição central (próxima ao Papai Noel)
                dst_elfo.x = WINDOW_WIDTH / 2 - 180;
                dst_elfo.y = 200 + (i == elfos_ajudando[0] ? 0 : (i == elfos_ajudando[1] ? 100 : 200));
            } else if (elfos[i].status == 1 || elfos[i].status == 2) {
                // Posição destacada para elfos que pedem ajuda
                dst_elfo.x = 80 + (i / 10) * 150;
                dst_elfo.y = 80 + (i % 10) * 100;
            } else {
                // Posição original
                dst_elfo.x = 50 + (i / 10) * 150; // Colunas de 10 elfos
                dst_elfo.y = 80 + (i % 10) * 100;
            }
            dst_elfo.w = 100;
            dst_elfo.h = 100;
        
            // Renderiza a textura correspondente ao status do elfo
            SDL_RenderCopy(renderer, (elfos[i].status ? tex_elfoajuda : tex_elfo), NULL, &dst_elfo);
        }

        // Renderizar o trenó se todas as renas estiverem engatadas
        if (treno_status) {
            SDL_Rect dst_treno = {WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 50, 400, 300}; // Ajuste a posição e o tamanho
            SDL_RenderCopy(renderer, tex_treno, NULL, &dst_treno);
        }

        // Renas
        for (int i = 0; i < NUM_OF_REINDEERS; i++) {
            SDL_Rect dst_rena;
            if (renas_status[i])
            {
                if (renas_papai) {
                    dst_rena.x = WINDOW_WIDTH / 2 + 300 + (i % 3) * 120;
                    dst_rena.y = 450 + (i / 3) * 120;
                } else {
                    dst_rena.x = WINDOW_WIDTH - 120;
                    dst_rena.y = 100 + i * 100;
                }
                dst_rena.w = 100;
                dst_rena.h = 100;
                SDL_RenderCopy(renderer, tex_rena, NULL, &dst_rena);
            }
        }
        pthread_mutex_unlock(&ui_mutex);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    // Limpeza
    SDL_DestroyTexture(tex_elfo);
    SDL_DestroyTexture(tex_elfoajuda);
    SDL_DestroyTexture(tex_papai_dorme);
    SDL_DestroyTexture(tex_papai_acorda);
    SDL_DestroyTexture(tex_rena);
    SDL_DestroyTexture(tex_treno);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
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