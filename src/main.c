#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define QUANT_ELFOS 10 // Quantidade de elfos no programa
#define CHANCE_PROBLEMA                                                        \
  10 // Chance de o elfo possuir um problema.
     // Exemplo: 10 significa que há uma chance
     // de 1 em 10 de um elfo possuir um problema,
     // 20 significa que há uma chance de 1 em 20, etc.

void prepareSleigh();
void getHitched();
void helpElves();
void getHelp();

// Essa função cuida do trabalho de todos os elfos. Os elfos que precisarem de
// ajuda irão para uma fila, os que não precisarem ficarão fora da fila
void *supervisor_elfos() {
  // A cada 1 segundo verificamos o status de cada elfo
  while (true) {
    sleep(1);
    for (int i = 0; i < QUANT_ELFOS; i++) {

      if (random() % CHANCE_PROBLEMA == 1) {
        //TODO:
        // Adicionar elfo na fila
      }
    }
  }
}

int main() {}
