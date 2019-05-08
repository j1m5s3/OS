#include <stdio.h>
#include <stdlib.h>
#include "sched_sim.h"

void initialize(){
	proc_array = (proc_info *)malloc(num_of_procs*sizeof(proc_info));
	arr = (int *)malloc(num_of_procs*3*sizeof(int));
	queue = (QUEUE *)malloc(sizeof(QUEUE));
	queue -> capacity = num_of_procs;
	queue -> front = queue -> size = 0;
	queue -> capacity = num_of_procs;
	queue -> rear = queue -> capacity - 1;
	queue -> array = (int *)malloc(queue -> capacity * sizeof(int));
}

void fcfs(int update_interval){
	int t = 0;
	int burst;
	int arrival_check = 0;
	int i;

	printf("****FCFS SCHEDULING****\n");

	for(i = 0; i < num_of_procs; i++){//loop for processes
		printf("t = %d\n", t);
		printf("CPU: Loading process %d (CPU BURST = %d)\n", i, proc_array[i].cpu_burst);
		burst = proc_array[i].cpu_burst
		while(burst > 0){
			while(arrival_check < num_of_procs){
				if(proc_array[arrival_check].arrival_time == t){
					enqueue(arrival_check);//left off here 

		
	}

}

void enqueue(int arrival_check){//left off here
	queue -> rear = (queue -> rear + 1) % queue -> capacity 
}
