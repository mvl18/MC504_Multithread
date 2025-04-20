#include "santa.h"

void santa(){

  while (true) {

    // TODO: preencher essa condicional com o semáforo das renas
    if (false) {
      // do something
    }
    else if (sem_trywait(&semaforo_acordar_santa)) {
      helpElves();
    }
  
  }

}

void helpElves(){
  sleep(rand()%5);

  printf("Santa terminou de ajudar esses elfos");

  sem_post(&semaforo_ajuda_finalizada);
  sem_post(&semaforo_ajuda_finalizada);
  sem_post(&semaforo_ajuda_finalizada);
}
