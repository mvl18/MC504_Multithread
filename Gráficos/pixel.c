#include <ncurses.h>

int heart[8][8] = {
    {0,1,1,0,0,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
};

int main() {
    initscr();
    start_color();          // Ativa cores
    curs_set(0);
    noecho();

    init_pair(1, COLOR_RED, COLOR_RED);   // pixel vermelho
    init_pair(2, COLOR_BLACK, COLOR_BLACK); // pixel vazio

    int start_y = 5;
    int start_x = 10;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int color = heart[y][x] ? 1 : 2;
            attron(COLOR_PAIR(color));
            mvprintw(start_y + y, start_x + x * 2, "  ");  // "  " para parecer quadrado
            attroff(COLOR_PAIR(color));
        }
    }

    refresh();
    getch();
    endwin();
    return 0;
}
