#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched_sim.h"


int main(int argc, char* argv[]){

	FILE *input_file = fopen(argv[1], "r");//input file open for reading
	FILE *output_file = fopen(argv[2], "w");//output file open for writing
	int update_interval = atoi(argv[3]); //User selected update interval
	int burst_num = 0;
	int priority_num = 1;
	int arrival_num = 2;
	int i = 0;
	int j;

	if(strcmp(argv[1], "testin1.dat") == 0){
		num_of_procs = 5;
	}
	else{ 
		num_of_procs = 10;
	}
	
	initialize();

	while(fscanf(input_file, "%d",&arr[i]) == 1){
		i++;
	}

	for(j = 0; j < num_of_procs; j++){
		proc_array[j].cpu_burst = arr[burst_num];
		burst_num = burst_num + 3;
		proc_array[j].priority = arr[priority_num];
		priority_num = priority_num + 3;
		proc_array[j].arrival_time = arr[arrival_num];
		arrival_num = arrival_num + 3;
	}

	printf("BURST	PRIORITY	ARRIVAL\n");

	for(j = 0; j < num_of_procs; j++){
		printf("%d	%d		%d\n", proc_array[j].cpu_burst, proc_array[j].priority, proc_array[j].arrival_time);
	}
	fcfs(update_interval);
	return 0; 
}

