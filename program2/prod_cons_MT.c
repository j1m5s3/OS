#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "prod_cons_MT.h"

void* producer(void *current_producer){//producer function 
	int counter_prod;//loop counter
	int item;//item to be put on the buffer
	int cur_prod = (*(int *)current_producer);//current producer number

	printf("P%d: Producing %d values\n", cur_prod, 2 * buff.buffersize);

	for(counter_prod = 0; counter_prod < (2 * buff.buffersize); counter_prod++){//Put items on the buffer
		if(buff.items_in_buff == buff.buffersize){
			printf("P%d: Blocked due to full buffer\n", cur_prod);
			sem_wait(&empty);//wait for buffer to be empty 
			printf("P%d: Done waiting on full buffer\n", cur_prod);
		}
		sem_wait(&mutex);//wait for lock to be available

		item = rand() % (10+1);//create random number 1-10
		buff.buffer[buff.next_in] = item;
		buff.items_in_buff++;
		printf("P%d: Writing %d to position %d\n", cur_prod, item, buff.next_in); 
		buff.next_in = (buff.next_in + 1) % buff.buffersize;//calculate next_in index
		
		sem_post(&mutex);//release lock
		sem_post(&data);//update amount of data in the buffer
	}
	printf("P%d: Exiting\n", cur_prod);
}

void* consumer(void *current_consumer){
	int counter_cons;//loop counter
	int item;
	int cur_con = (*(int *)current_consumer);//current producer number
	int data_consuming;//amount of data consumer thread is consuming

	if(cur_con == buff.n_cons){//last thread sill take excess data if there is any
		data_consuming = last_c_data_share;
	}
	else {
		data_consuming = data_per_cons_min;
	}

	printf("C%d: Consuming %d values\n", cur_con, data_consuming);

	for(counter_cons = 0; counter_cons < data_consuming; counter_cons++){//consume data
		if(buff.items_in_buff == 0){
			printf("C%d blocked due to empty buffer\n", cur_con);
			sem_wait(&data);//waits for data to be on the buffer
			printf("C%d: Done waiting for empty buffer\n", cur_con);
		}
		sem_wait(&mutex);//wait for lock

		item = buff.buffer[buff.next_out];//take item from buffer 
		buff.items_in_buff--;
		printf("C%d: Reading %d from postion %d\n", cur_con, item, buff.next_out);
		buff.next_out = (buff.next_out +1) % buff.buffersize;

		sem_post(&mutex);//release lock
		sem_post(&empty);//update the amount of empty slots
	}
	printf("C%d: Exiting\n", cur_con);
}

void initialize(){
	prods_id = (pthread_t *)malloc(buff.n_prods*sizeof(pthread_t));//initialize prod thread id array
	cons_id = (pthread_t *)malloc(buff.n_prods*sizeof(pthread_t));//initialize con thread id array

	buff.buffer = (int *)malloc(buff.buffersize*sizeof(int));//initialize buffer 
	buff.next_in = 0;
	buff.next_out = 0;
	//initialize mutex lock, and buffer status semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, buff.buffersize);//empty spaces in the buffer
	sem_init(&data, 0, 0);//amount of data in the buffer
}
	
