#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Para compilar use -lSDL2 -lSDL2_image

#define MAX_RENAS 100
#define MAX_ELFOS 10

typedef struct {
    int status; // 0 = normal, 1 = ajudando
} Elfo;

typedef struct {
    int x, y;
} Posicao;

SDL_Texture* carregar_textura(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path);
    if (!tex) {
        printf("Erro ao carregar imagem %s: %s\n", path, IMG_GetError());
    }
    return tex;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    SDL_Window* window = SDL_CreateWindow("Oficina do Papai Noel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int WINDOW_WIDTH, WINDOW_HEIGHT;
    SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

    // Carregar as texturas
    SDL_Texture* tex_elfo = carregar_textura("elfo.png", renderer);
    SDL_Texture* tex_elfoajuda = carregar_textura("elfoajuda.png", renderer);
    SDL_Texture* tex_papai_dorme = carregar_textura("papainoeldorme.png", renderer);
    SDL_Texture* tex_papai_acorda = carregar_textura("papainoel.png", renderer);
    SDL_Texture* tex_rena = carregar_textura("rena.png", renderer);
    SDL_Texture* tex_titulo = carregar_textura("titulo.png", renderer);

    if (!tex_elfo || !tex_elfoajuda || !tex_papai_dorme || !tex_papai_acorda || !tex_rena || !tex_titulo) {
        printf("Erro ao carregar imagens.\n");
        return -1;
    }

    // Inicializa os elfos
    Elfo elfos[MAX_ELFOS] = {0};
    int renas = 0;
    Posicao pos_renas[MAX_RENAS];

    Uint32 ultimoTempoRena = SDL_GetTicks();
    Uint32 ultimoTempoElfo = SDL_GetTicks();
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        Uint32 agora = SDL_GetTicks();

        // Adiciona uma rena a cada 1 segundo
        if (agora - ultimoTempoRena >= 1000 && renas < 9) {
            pos_renas[renas].x = WINDOW_WIDTH - 100;
            pos_renas[renas].y = 50 + (renas * 60);
            renas++;
            ultimoTempoRena = agora;
        }

        // Altera o status dos elfos a cada 1 segundo
        if (agora - ultimoTempoElfo >= 1000) {
            int i = rand() % 5;
            if (elfos[i].status == 0) {
                elfos[i].status = 1;
            }
            ultimoTempoElfo = agora;
        }

        // Contar elfos ajudando
        int ajudando = 0;
        for (int i = 0; i < MAX_ELFOS; i++) {
            if (elfos[i].status == 1) ajudando++;
        }

        // Renderização
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Título
        SDL_Rect rect_titulo = {WINDOW_WIDTH / 2 - 350, 20, 700, 100}; // (posição horizontal apartir da lateral, posição vertical apartir do topo, largura do retangulo, altura do retangulo)
        SDL_RenderCopy(renderer, tex_titulo, NULL, &rect_titulo);

        // Papai Noel
        SDL_Rect dst_papai = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 150, 300, 300};
        SDL_RenderCopy(renderer, (ajudando >= 3 ? tex_papai_acorda : tex_papai_dorme), NULL, &dst_papai);

        // Elfos
        int ajudando_count = 0;
        int elfos_ajudando[3] = {-1, -1, -1}; // Armazena os índices dos 3 elfos ajudando
            
        // Verifica quais elfos estão ajudando e armazena os primeiros 3
        for (int i = 0; i < MAX_ELFOS; i++) {
            if (elfos[i].status == 1) {
                if (ajudando_count < 3) {
                    elfos_ajudando[ajudando_count] = i; // Salva o índice do elfo ajudando
                }
                ajudando_count++;
            }
        }
        
        // Renderização dos elfos
        for (int i = 0; i < MAX_ELFOS; i++) {
            SDL_Rect dst_elfo;
        
            // Apenas desloca os 3 primeiros elfos ajudando quando ajudando_count == 3
            if (ajudando_count == 3 && 
                (i == elfos_ajudando[0] || i == elfos_ajudando[1] || i == elfos_ajudando[2])) {
                dst_elfo.x = WINDOW_WIDTH / 2 - 150; // Perto do centro da tela
                dst_elfo.y = 200 + (i == elfos_ajudando[0] ? 0 : (i == elfos_ajudando[1] ? 100 : 200)); // Espaçamento vertical
            } else {
                // Elfos restantes permanecem em suas posições originais
                dst_elfo.x = 50;
                dst_elfo.y = 200 + i * 100;
            }
        
            dst_elfo.w = 100;
            dst_elfo.h = 100;
        
            // Renderiza a textura correspondente ao status do elfo
            SDL_RenderCopy(renderer, (elfos[i].status ? tex_elfoajuda : tex_elfo), NULL, &dst_elfo);
        }

        // Renas
        for (int i = 0; i < renas; i++) {
            SDL_Rect dst_rena;
            if (renas >= 9) {
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

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
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
