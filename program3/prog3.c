#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched_sim.h"

const int QUANTUM = 2;

int main(int argc, char* argv[]){

	FILE *input_file;
	FILE *output_file;

	PROC_INFO *proc;

	int num_of_procs;
	int update_interval;
	int algorithm;//0,1,2,3,4 (FCFS,SJF,STCF,RR,NPP)
	int *temp_arr;//temporary array to hold contents of input file
	int j;
	int k;
	int i = 0;//used for index of temp_arr  
	int burst = 0;
	int priority_num = 1;
	int arrival = 2;
	printf("MAIN: %d\n",__LINE__);	
	SUMMARY **algorithm_summaries = (SUMMARY **)malloc(sizeof(SUMMARY *)*5);
	
	input_file = fopen(argv[1], "r");
	output_file = fopen(argv[2], "w");
	update_interval = atoi(argv[3]);

	if(strcmp(argv[1], "testin1.dat") == 0){
		num_of_procs = 5;
	}
	else{
		num_of_procs = 10;
	}
	printf("%d\n",num_of_procs);
	printf("MAIN: %d\n", __LINE__);

	temp_arr = (int *)malloc(num_of_procs*3*sizeof(int));
	printf("MAIN: %d\n", __LINE__);
	while(fscanf(input_file, "%d", &temp_arr[i]) == 1){
		i++;
	}
	printf("MAIN: %d\n",__LINE__);
	proc = (PROC_INFO *)malloc(num_of_procs*sizeof(PROC_INFO));
	printf("MAIN: %d\n",__LINE__);
	for(j = 0; j < num_of_procs; j++){

		proc[j].burst_t = temp_arr[burst];
		burst = burst + 3;
		proc[j].priority = temp_arr[priority_num];
		priority_num = priority_num + 3;
		proc[j].arrival_t = temp_arr[arrival];
		arrival = arrival + 3;
		proc[j].id = j;
		proc[j].status = 'N';//Processis NEW initially
		proc[j].remaining_t = proc[j].burst_t;
		proc[j].start_t = -1;
		proc[j].end_t = -1;
		proc[j].last_run = -1;
		proc[j].tt = -1;
		proc[j].wt = -1;
	}


	for(algorithm = 0; algorithm < 4; algorithm++){
		printf("MAIN: %d\n",__LINE__);

		algorithm_summaries[algorithm] = simulation(output_file, update_interval, proc, num_of_procs, algorithm);
		printf("MAIN: %d\n",__LINE__);

		algorithm_results(output_file, proc, num_of_procs, algorithm_summaries, algorithm);
		printf("MAIN: %d\n",__LINE__);

	}

	print_simulation_results(output_file, algorithm_summaries);

	fclose(input_file);
	fclose(output_file);
	free(proc);

	for(k = 0; k < 5; k++){
		destroy_SUMMARY(algorithm_summaries[k]);
	}

	free(algorithm_summaries);

	return 0;
}


			       	       
