#include <ncurses.h>
#include <stdio.h>
#include <string.h>


int main() {
    // Inicializa o modo ncurses
    initscr();              // Inicia a tela
    cbreak();               // Desabilita o buffer de linha (input imediato)
    noecho();               // Não mostra o que o usuário digita
    curs_set(0);            // Esconde o cursor

    int height, width;
    getmaxyx(stdscr, height, width);  // Pega as dimensões da tela

    char *mensagem = "PROBLEMA DO PAPAI NOEL";
    int start_x = (width - strlen(mensagem)) / 2;
    int start_y = height / 20;

    mvprintw(start_y, start_x, "%s", mensagem);  // Escreve no centro da tela
    refresh();              // Atualiza a tela

    getch();                // Espera uma tecla
    endwin();               // Sai do modo ncurses

    return 0;
}
