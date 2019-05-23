#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched_sim.h"


int main(int argc, char* argv[]){

	input_file = fopen(argv[1], "r");//input file open for reading
	output_file = fopen(argv[2], "w");//output file open for writing
	update_interval = atoi(argv[3]); //User selected update interval

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

	for(j = 0; j < num_of_procs; j++){//load procs info
		proc_array[j].cpu_burst = arr[burst_num];
		burst_num = burst_num + 3;
		proc_array[j].priority = arr[priority_num];
		priority_num = priority_num + 3;
		proc_array[j].arrival_time = arr[arrival_num];
		arrival_num = arrival_num + 3;
		proc_array[j].ID = j;
		proc_array[j].status = 'N';//New procs
		proc_array[j].remaining_t = proc_array[j].cpu_burst;
	}

	fcfs(update_interval);
	fclose(input_file);
	fclose(output_file);

	free(queue);
	free(ll);
	free(summary);
	return 0; 
}

