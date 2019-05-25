/*
 * James Lynch
 * Program 3: CPU SCHEDULING SIMULATOR (FCFS,SJF,STCF,RR,NPP)
 * 5/25/19
 * Operating Systems Spring 19 
 * Dr. Geiger
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched_sim.h"

const int QUANTUM = 2;

int main(int argc, char* argv[]){

	FILE *input_file;//pointer to input file (testin1.dat/testin2.dat)
	FILE *output_file;//pointer to output file (output1.text/outpue2.text)

	PROC_INFO *proc;//pointer to PROC_INFO struct which holds info for each process

	int num_of_procs;
	int update_interval;//interval in which scheduler will update to file
	int algorithm;//0,1,2,3,4 (FCFS,SJF,STCF,RR,NPP)
	int *temp_arr;//temporary array to hold contents of input file
	int j;
	int k;
	int i = 0;//used for index of temp_arr  
	int burst = 0;//position of CPU BURST in input file
	int priority_num = 1;//position of PRIORITY# in input file
	int arrival = 2;

	SUMMARY **algorithm_summaries = (SUMMARY **)malloc(sizeof(SUMMARY *)*5);//double pointer will reference to each individual algo summary
	
	input_file = fopen(argv[1], "r");
	output_file = fopen(argv[2], "w");
	update_interval = atoi(argv[3]);

	if(strcmp(argv[1], "testin1.dat") == 0){//simple check of file name to determine # of processes
		num_of_procs = 5;
	}
	else{
		num_of_procs = 10;
	}

	temp_arr = (int *)malloc(num_of_procs*3*sizeof(int));//temporary storage for process info

	while(fscanf(input_file, "%d", &temp_arr[i]) == 1){//grab from file and put in to temp storage
		i++;
	}
	
	proc = (PROC_INFO *)malloc(num_of_procs*sizeof(PROC_INFO));//process array will allow for easy access of each process info 
	
	for(j = 0; j < num_of_procs; j++){//loop puts the process information in to proc based on j index (j = process id)

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


	for(algorithm = 0; algorithm < 5; algorithm++){//will increment each time and algorithm is completed

		algorithm_summaries[algorithm] = simulation(output_file, update_interval, proc, num_of_procs, algorithm);//simulation will return a ptr to the corresponding algorithm summary
		algorithm_results(output_file, proc, num_of_procs, algorithm_summaries, algorithm);//print the results of the most recently completed algorithm

	}

	print_simulation_results(output_file, algorithm_summaries);//print complete simulation results 

	fclose(input_file);
	fclose(output_file);
	free(proc);

	for(k = 0; k < 5; k++){//destroy each algorithm summary stored in algorithm_summaries
		destroy_SUMMARY(algorithm_summaries[k]);
	}

	free(algorithm_summaries);

	return 0;
}


			       	       
