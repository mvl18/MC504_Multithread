#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Para compilar use -lSDL2 -lSDL2_image

#define MAX_RENAS 10
#define MAX_ELFOS 10

typedef struct {
    int status; // 0 = trabalhando, 1 = na fila, 2 = ajudado, 3 = finalizado
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

int rrandom(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// Inicializa os elfos
Elfo elfos[MAX_ELFOS] = {0};
int elfos_ajudando[3] = {-1, -1, -1}; // Armazena os índices dos 3 elfos ajudando
int elfo_ajuda = 0;
int santa_status;
int renas_status[MAX_RENAS] = {0};
int renas_papai = 0;

void acorda_rena(int id_rena){
    renas_status[id_rena] = 1;
}
 
void elfo_atualiza(int id_elfo, int novo_status){
    elfos[id_elfo].status = novo_status;
}
void ajuda_elfo(int id_elfo){
    elfos[id_elfo].status = 1;
}
void santa_libera_elfos(){
    for (int j = 0; j < 3; j++)
    {
        elfos[elfos_ajudando[j]].status = 0;
        elfos_ajudando[j] = -1;
    }
    elfo_ajuda = 0;
}
void santa_chama_elfos(){
    elfo_ajuda = 1;
}
void santa_novo_status(){

}


void santa_verifica(){
    int ajudando_count = 0;
        
    // Verifica quais elfos estão ajudando e armazena os primeiros 3
    if(!elfo_ajuda){
        for (int i = 0; i < MAX_ELFOS; i++) {
            if (elfos[i].status == 1 || elfos[i].status == 2) {
                if (ajudando_count < 3) {
                    elfos[i].status = 2;
                    elfos_ajudando[ajudando_count] = i; // Salva o índice do elfo ajudando
                }
                ajudando_count++;
            }
        }
    }
    int renas_todas = 1;
    for (int i = 0; i < MAX_RENAS; i++)
    {
        renas_todas &= renas_status[i];
    }
    if (!elfo_ajuda && renas_todas && !renas_papai){
        renas_papai = 1;
    }
    printf("%d\n", ajudando_count);
    if (ajudando_count == 3 && !renas_papai)
    {
        santa_chama_elfos(elfos_ajudando);
    }
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

    Uint32 ultimoTempoRena = SDL_GetTicks();
    Uint32 ultimoTempoElfo = SDL_GetTicks();
    int quit = 0;
    SDL_Event e;
    int aux = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                quit = 1;
            }
        }

        Uint32 agora = SDL_GetTicks();

        int renas_todas = 1;
        for (int i = 0; i < MAX_RENAS; i++)
        {
            renas_todas &= renas_status[i];
        }
        // Adiciona uma rena a cada 1 segundo
        if (agora - ultimoTempoRena >= 1000 && !renas_todas) {
            int k = rrandom(0, MAX_RENAS-1);
            while(renas_status[k]){
                k = rrandom(0, MAX_RENAS-1);
            }
            if(k >= 10){
            }
            acorda_rena(k);
            ultimoTempoRena = agora;
        }

        
        // Altera o status dos elfos a cada 1 segundo

        
        if (agora - ultimoTempoElfo >= 1000)
        {

            int i = rand() % MAX_ELFOS;
            int k = 0;
            while (elfos[i].status && k++ != MAX_ELFOS)
            {
                i = rand() % MAX_ELFOS;
            }
            ajuda_elfo(i);

            aux+= 1;
            if (elfo_ajuda && aux % 3 == 0)
                santa_libera_elfos();
            ultimoTempoElfo = agora;
        }

        // Contar elfos ajudando
        santa_verifica();

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


        // Renderização dos elfos
        for (int i = 0; i < MAX_ELFOS; i++) {
            SDL_Rect dst_elfo;
        
            // Apenas desloca os 3 primeiros elfos ajudando quando ajudando_count == 3
            if (elfo_ajuda && 
                (i == elfos_ajudando[0] || i == elfos_ajudando[1] || i == elfos_ajudando[2])) {
                dst_elfo.x = WINDOW_WIDTH / 2 - 150;                                                    // Perto do centro da tela
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
        for (int i = 0; i < MAX_RENAS; i++) {
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
