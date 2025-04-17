#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fila.h>

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

fila_t fila_elfos;

// O Elfo pode estar em 3 condições
//  1. Trabalhando
//  2. Esperando para ser ajudado
//  3. Sendo ajudado
void elfo() {
  while (true) {
    sleep(1);

    // Verificar se o elfo está com problema
    if (random() % CHANCE_PROBLEMA == 1) {
      // O elfo entra na fila
    }
  }
}

int main() {
  // Inicializando a fila de elfos
  fila_elfos = fila_init(QUANT_ELFOS);

}
