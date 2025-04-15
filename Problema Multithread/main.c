#include <stdio.h>
#include <pthread.h>
#include <stdlib.>

#define QUANT_ELFOS 10

void prepareSleigh();
void getHitched();
void helpElves();
void getHelp();

bool elfos[QUANT_ELFOS] = {0}

// Essa função cuida do trabalho de todos os elfos. Cada elfo terá uma flag:
//  - trabalhando: 0
//  - com problema: 1
// com base nisso, eles irão pedir ajuda para o Papai Noel quando houverem três deles
void* supervisor_elfos(){

}

int main() {
    pthread_t threads[5];
    int ids[5];

    for (int i = 0; i < 5; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, print_hello, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
