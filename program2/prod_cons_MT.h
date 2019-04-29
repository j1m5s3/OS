#ifndef PROD_CONS_MT_H_
#define PROD_CONS_MT_H_

sem_t mutex;//Binary sem for mutual exclusion
sem_t empty;//Empty slots for producer
sem_t data;//Data in buffer for consumer

pthread_t *prods_id;//pointer to producer thread ids
pthread_t *cons_id;//pointer to consumer thread ids

int data_per_cons_min;//minimum data per consumer
int last_c_data_share;//data share for the last consumer

typedef struct  bbuffer{//structure that defines the buffer 
	int *buffer;
	int buffersize;
	int next_in;//Back of line
	int next_out;//Front of line
	int n_prods;
	int n_cons;
	int items_in_buff;
}bb;

bb buff;//accessor to bbbuffer struct

void* producer(void *current_producer);
void* consumer(void *current_consumer);
void initialize();//initialize arrays, allocate memory for thread ids and buffer

#endif
