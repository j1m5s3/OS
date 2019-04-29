/*
 * Name: James Lynch
 * Date: 4/20/19
 * Instructor: Dr Gieger
 * OS Spring 19
 * Description: Program utilizes threads to simulate produce consumer interaction
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "prod_cons_MT.h"


int main(int argc, char* argv[]){
	
	int total_data;//total data to be consumed and produced
	int producers;//producer loop counter
	int consumers;//consumer loop counter
	int *current_producer = malloc(sizeof(*current_producer));//producer function param
	int *current_consumer = malloc(sizeof(*current_consumer));//consumer function param
	int join_prods = 0;//producer thread join counter
	int join_cons = 0;//consumer thread join counter

	buff.items_in_buff = 0;//initially 0 intems in the buffer

	srand(time(NULL));//Seed to create random number 1-10	

	//cmd line args
	buff.buffersize = atoi(argv[1]);
	buff.n_prods = atoi(argv[2]);
	buff.n_cons = atoi(argv[3]);

	//Calculate the total data and the amount of data each consumer takes
	total_data = buff.n_prods * 2 * buff.buffersize;
	data_per_cons_min = total_data / buff.n_cons;
	last_c_data_share = total_data - ((buff.n_cons - 1) * data_per_cons_min);
	
	//Initialize semaphores, buffer, and pid/cid arrays
	initialize();

	for(producers = 0; producers < buff.n_prods; producers++){//create producer threads
		printf("MAIN: Started P%d\n", producers);
		current_producer = malloc(sizeof(*current_producer));//each prod gets a pointer to a thread number
		*current_producer = producers;
		pthread_create(&prods_id[producers], NULL, producer, current_producer);
	}

	for(consumers = 0; consumers < buff.n_cons; consumers++){//create consumer threads
		printf("MAIN: Started C%d\n", consumers);
		current_consumer = malloc(sizeof(*current_consumer));//each con thread gets a pointer to a thread number
		*current_consumer = consumers;
		pthread_create(&cons_id[consumers], NULL, consumer, current_consumer);
	}

	while(join_prods < producers){//join prod threads
		pthread_join(prods_id[join_prods], NULL);//wait for referenced prod id to finish
		printf("MAIN: P%d joined\n", join_prods);
		join_prods++;
	}

	while(join_cons < consumers){//join con threads
		pthread_join(cons_id[join_cons], NULL);//wait for referenced con id to finish
		printf("MAIN: C%d joined\n", join_cons);
		join_cons++;
	}

	printf("MAIN: PROGRAM COMPLETE\n");

	return 0;

}
