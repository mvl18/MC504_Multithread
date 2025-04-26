#include<fifo_sem.h>
 
void fifo_sem_init(fifo_sem_t *sem, int start){
	sem_init(&sem->semaforo, 0, start);
	sem->fila = fila_init(QUANT_ELFOS);
}

void fifo_sem_post(fifo_sem_t *sem){
  sem_post(&sem->semaforo); 

}

void fifo_sem_wait(fifo_sem_t *sem, int id){
  fila_append(sem->fila, id);
  while (true){
    sem_wait(&sem->semaforo);
    if ((fila_peek(sem->fila, 0) == id)) { 
      break;
    } else {
      fifo_sem_post(sem);
    }
  }
  fila_pop(sem->fila);
}

void fifo_sem_destroy(fifo_sem_t *sem){

  fila_destroy(sem->fila);
}
