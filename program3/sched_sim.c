#include <stdio.h>
#include <stdlib.h>
#include "sched_sim.h"

QUEUE* create_Q(){

	QUEUE *queue = (QUEUE *)malloc(sizeof(QUEUE));

	queue -> head = NULL;
	queue -> tail = NULL;

	return queue;
}

LL* create_LL(){

	LL *linked_l = (LL *)malloc(sizeof(LL));

	linked_l -> head = NULL;
	linked_l -> tail = NULL;

	return linked_l;
}

SUMMARY* create_SUMMARY(){

	SUMMARY *algorithm_summary = (SUMMARY *)malloc(sizeof(SUMMARY));

	algorithm_summary -> avg_wt = 0;
	algorithm_summary -> avg_tt = 0;
	algorithm_summary -> proc_seq = create_LL();
	algorithm_summary -> context_switches = 0;

	return algorithm_summary;
}

void destroy_Q(QUEUE* queue){//destroy queue after completion of each algorithm

	NODE *current = queue -> head;

	while(current != NULL){

		current = queue -> head -> next;
		free(queue -> head);
		queue -> head = current;
	}

	free(queue);
	queue = NULL;
}

void destroy_LL(LL *linked_l){//destroy linked list

	LL_NODE *current = linked_l -> head;

	while(current != NULL){

		current = linked_l -> head -> next;
		free(linked_l -> head);
		linked_l -> head = current;
	}

	free(linked_l);
	linked_l = NULL;
}

void destroy_SUMMARY(SUMMARY *algorithm_summary){//destroy summary after program is finished

	destroy_LL(algorithm_summary -> proc_seq);//destroy linked list holding process sequence for each algorithm
	free(algorithm_summary);
	algorithm_summary = NULL;
}

void enQ(QUEUE *queue, PROC_INFO *proc){//simple enqueue a process when it has arrived (RR and FCFS)
	

	NODE *NEWnode = (NODE *)malloc(sizeof(NODE));

	NEWnode -> info = proc;
	NEWnode -> next = NULL;

	if(Qempty(queue)){
		
		queue -> head = NEWnode;
		queue -> tail = NEWnode;
	}
	else{
		queue -> tail -> next = NEWnode;
		queue -> tail = NEWnode;
	}
}

void burst_enQ(QUEUE *queue, PROC_INFO *proc){//enqueue process depending on burst

	NODE *CURRENTnode;
	NODE *NEWnode = (NODE *)malloc(sizeof(NODE));

	NEWnode -> info = proc;
	NEWnode -> next = NULL;

	if(Qempty(queue)){

		queue -> head = NEWnode;
		queue -> tail = NEWnode;
		return;
	}

	CURRENTnode = queue -> head;

	if(queue -> head -> info -> burst_t > proc -> burst_t){

		NEWnode -> next = queue -> head;
		queue -> head = NEWnode;
	}
	else{
		while(CURRENTnode -> next != NULL && CURRENTnode -> next -> info -> burst_t < proc -> burst_t){//determine which process in the queue has least remaing burst and place accordingly

			CURRENTnode = CURRENTnode -> next;
		}

		if(queue -> tail == CURRENTnode){
			queue -> tail = NEWnode;
		}

		NEWnode -> next = CURRENTnode -> next;
		CURRENTnode -> next = NEWnode;
	}
}

void remainingBURST_enQ(QUEUE *queue, PROC_INFO *proc){//enqueue a process based on remaining burst time of the process

	NODE *CURRENTnode;
	NODE *NEWnode = (NODE *)malloc(sizeof(NODE));

	NEWnode -> info = proc;
	NEWnode -> next = NULL;

	if(Qempty(queue)){

		queue -> head = NEWnode;
		queue -> tail = NEWnode;
		return;
	}

	CURRENTnode = queue -> head;

	if(queue -> head -> info -> remaining_t > proc -> remaining_t){//processes position in queue is based on the reamaing burst (remaining_t)

		NEWnode -> next = queue -> head;
		queue -> head = NEWnode;
	}
	else{
		while(CURRENTnode -> next != NULL && CURRENTnode -> next -> info -> remaining_t < proc -> remaining_t){

			CURRENTnode = CURRENTnode -> next;
		}

		if(queue -> tail == CURRENTnode){
			queue -> tail = NEWnode;
		}

		NEWnode -> next = CURRENTnode -> next;
		CURRENTnode -> next = NEWnode;
	}
}

void priority_enQ(QUEUE *queue, PROC_INFO *proc){//enqueue a process based on the coprresponding priority# of that process

	NODE *CURRENTnode;
	NODE *NEWnode = (NODE *)malloc(sizeof(NODE));

	NEWnode -> info = proc;
	NEWnode -> next = NULL;

	if(Qempty(queue)){

		queue -> head = NEWnode;
		queue -> tail = NEWnode;
		return;
	}

	CURRENTnode = queue -> head;

	if(queue -> head -> info -> priority > proc -> priority){
		
		NEWnode -> next = queue -> head;
		queue -> head = NEWnode;
	}
	else{
		while(CURRENTnode -> next != NULL && CURRENTnode -> next -> info -> priority < proc -> priority){//postition in the queue is based on priority of process
			
			CURRENTnode = CURRENTnode -> next;
		}

		if(queue -> tail == CURRENTnode){
			queue -> tail = NEWnode;
		}

		if(queue -> tail == CURRENTnode){
			queue -> tail = NEWnode;
		}

		NEWnode -> next = CURRENTnode -> next;
		CURRENTnode -> next = NEWnode;
	}
}

int deQ(QUEUE *queue){//dequeue a process from the queue when it has become Active

	int id;
	NODE *TEMPnode;//temporary node will hold the current head of the queue

	if(Qempty(queue)){
		return -1;
	}
	
	TEMPnode = queue -> head;
	id = TEMPnode -> info -> id;

	queue -> head = queue -> head -> next;

	free(TEMPnode);//previous head of the queue removed

	if(queue -> head == NULL){
		queue -> tail = NULL;
	}

	return id;
}

int checkQ(QUEUE *queue){//grab the id of the head of the queue (next in)
	return queue -> head -> info -> id;
}

int Qempty(QUEUE *queue){//check if queue is empty
	return queue -> head == NULL && queue -> tail == NULL;
}

void printQ(FILE *output_file, QUEUE *queue){//print the queue to show the processes that have arrived

	NODE CURRENTnode;

	if(Qempty(queue)){
		fprintf(output_file, "EMPTY\n");
		return;
	}

	CURRENTnode.info = queue -> head -> info;
	CURRENTnode.next = queue -> head -> next;

	while(CURRENTnode.next != NULL){//print the queue 

		fprintf(output_file, "%d-", CURRENTnode.info -> id);
		CURRENTnode.info = CURRENTnode.next -> info;
		CURRENTnode.next = CURRENTnode.next -> next;
	}
	fprintf(output_file, "%d\n", queue -> tail -> info -> id);//print the last member of the queue (queue->tail->next = NULL)
}

void add_LLnode(LL *linked_l, int id){//add a new node to the linked list each time a process becomes Active

	LL_NODE *NEWnode = (LL_NODE *)malloc(sizeof(LL_NODE));

	NEWnode -> proc_id = id;
	NEWnode -> next = NULL;

	if(LLempty(linked_l)){
		
		linked_l -> head = NEWnode;
		linked_l -> tail = NEWnode;
	}
	else{
		linked_l -> tail -> next = NEWnode;
		linked_l -> tail = NEWnode;
	}
}

void printLL(FILE *output_file,LL *linked_l){//print the linked list to show process sequence

	LL_NODE CURRENTnode;

	if(LLempty(linked_l)){

		fprintf(output_file, "EMPTY\n");
		return;
	}

	CURRENTnode.proc_id = linked_l -> head -> proc_id;
	CURRENTnode.next = linked_l -> head -> next;

	while(CURRENTnode.next != NULL){//print the process sequence 
		
		fprintf(output_file, "%d-", CURRENTnode.proc_id);
		CURRENTnode.proc_id = CURRENTnode.next -> proc_id;
		CURRENTnode.next = CURRENTnode.next -> next;
	}
	fprintf(output_file, "%d\n", linked_l -> tail -> proc_id);//print the tail id (tail -> next = NULL)
}

int LLempty(LL *linked_l){//check if linked list is empty
	return linked_l -> head == NULL && linked_l -> tail == NULL;
}

SUMMARY* simulation(FILE *output_file, int update_interval, PROC_INFO *proc, int num_of_procs, int algorithm){//Simulation function in which the simulation of each algorithm takes place

	int i;
	int t = 0;
	int complete = 0;
	int curr_proc = -1;
	
	SUMMARY *algorithm_summary = create_SUMMARY();//initialize space for each algorithm summary
	QUEUE *queue = create_Q();//initialze queue upon the start of each algorithm

	if(algorithm > 0){//reset process info for each algorithm after FCFS
		for(i = 0; i < num_of_procs; i++){
			proc[i].remaining_t = proc[i].burst_t;
			proc[i].start_t = -1;
			proc[i].end_t = -1;
			proc[i].last_run = -1;
			proc[i].wt = -1;
			proc[i].tt = -1;
			proc[i].status = 'N';
		}
	}
	
	if(algorithm == 0)
		fprintf(output_file, "***** FCFS Scheduling *****\n");
	if(algorithm == 1)
		fprintf(output_file, "***** SJF Scheduling *****\n");
	if(algorithm == 2)
		fprintf(output_file, "***** STCF Scheduling *****\n");
	if(algorithm == 3)
		fprintf(output_file, "***** RR Scheduling *****\n");
	if(algorithm == 4)
		fprintf(output_file, "***** NPP Scheduling *****\n");

	while(complete < num_of_procs){
		for(i = 0; i < num_of_procs; i++){
			if(t >= proc[i].arrival_t){
				if(proc[i].status == 'N' || proc[i].status == 'W'){//processes are enqueued based on the current algorithm
					if(algorithm == 0){//FCFS
						enQ(queue, &proc[i]);
						proc[i].status = 'R';
					}
					if(algorithm == 1){//SJF
						burst_enQ(queue, &proc[i]);
						proc[i].status = 'R';
					}
					if(algorithm == 2){//STCF
						remainingBURST_enQ(queue, &proc[i]);
						proc[i].status = 'R';
					}
					if(algorithm == 3){//RR
						enQ(queue, &proc[i]);
						proc[i].status = 'R';
					}
					if(algorithm == 4){//NPP
						priority_enQ(queue, &proc[i]);
						proc[i].status = 'R';
					}
				}
			}
		}
		if(curr_proc == -1 && !Qempty(queue)){//first process to be loaded for each algorithm
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Loading Process %d ", checkQ(queue));
				fprintf(output_file, "(CPU BURST = %d)\n", proc[checkQ(queue)].burst_t);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file,"\n");
			}

			curr_proc = deQ(queue);
			proc[curr_proc].status = 'A';//First process is active
			proc[curr_proc].start_t = t;
			proc[curr_proc].last_run = t;
			proc[curr_proc].remaining_t--;//burst time is decremented each cycle
			algorithm_summary -> context_switches++;//oncrement context switch each time a new process runs
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);//process will be added to the linked list to show the complete process sequence of the algorithm

		}
		else if(curr_proc == -1 && Qempty(queue)){//For the case in which arrival time is not 0 and to make sure queue is working properly
			if(t % update_interval == 0){
				fprintf(output_file,"t = %d\n", t);
				fprintf(output_file, "CPU: Waiting for processes\n");
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
		}
		else if(proc[curr_proc].remaining_t <= 0 && !Qempty(queue)){//current process is finishing and another process has alredy arrived
			if(t % update_interval == 0){	
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d; Loading Process %d ", curr_proc, checkQ(queue));
				fprintf(output_file, "(CPU BURST = %d)\n", proc[checkQ(queue)].remaining_t);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file,"\n");
			}

			proc[curr_proc].status = 'C';//indicate complete status
			proc[curr_proc].end_t = t;
			complete++;//increment complete
			curr_proc = deQ(queue);//dequeue the head of the queue (next process up)
			proc[curr_proc].status = 'A';

			if(proc[curr_proc].remaining_t == proc[curr_proc].burst_t){
				proc[curr_proc].start_t = t;
			}
			proc[curr_proc].last_run = t;
			proc[curr_proc].remaining_t--;
			algorithm_summary -> context_switches++;
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);
		}
		else if(proc[curr_proc].remaining_t <= 0 && Qempty(queue)){//When a process finishes
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d \n", curr_proc);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
			proc[curr_proc].status = 'C';//given Complete status
			proc[curr_proc].end_t = t;
			complete++;//increment to indicate a process has completed
			curr_proc = -1;//reset the currrent process index, current process index is determined by the next dequeue
		}
		else if(algorithm == 2 && !Qempty(queue) && proc[curr_proc].remaining_t > proc[checkQ(queue)].remaining_t){//STCF scheduler chooses current process based on remaining burst
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Preempting Process %d (Remaining CPU BURST = %d; ", curr_proc, proc[curr_proc].remaining_t);
				fprintf(output_file, "Loading Process %d (CPU BURST = %d)\n", checkQ(queue), proc[checkQ(queue)].remaining_t);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
			proc[curr_proc].status = 'W';
			curr_proc = deQ(queue);
			proc[curr_proc].status = 'A';

			if(proc[curr_proc].remaining_t == proc[curr_proc].burst_t){
				proc[curr_proc].start_t = t;
			}

			proc[curr_proc].last_run = t;
			proc[curr_proc].remaining_t--;
			algorithm_summary -> context_switches++;
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);
		}
		else if(algorithm == 3 && !Qempty(queue) && t - proc[curr_proc].last_run >= QUANTUM){//RR scheduler with QUNTUM = 2 (2s bursts)
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Preempting Process %d (Remaining CPU BURST = %d); ", curr_proc, proc[curr_proc].remaining_t);
				fprintf(output_file, "Loading Process %d (CPU BURST = %d)\n", checkQ(queue), proc[checkQ(queue)].remaining_t);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
			proc[curr_proc].status = 'W';//processes will have wait status until RR returns 
			curr_proc = deQ(queue);//dequeue process for a burst time of value QUANTUM
			proc[curr_proc].status = 'A';//process is given Active status  

			if(proc[curr_proc].remaining_t == proc[curr_proc].burst_t){
				proc[curr_proc].start_t = t;
			}

			proc[curr_proc].last_run = t;
			proc[curr_proc].remaining_t--;
			algorithm_summary -> context_switches++;
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);//add a node to the linked list after each QUANTUM burst is completed
		}
		else{//while a process is running the program will enter
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Running Process %d ", curr_proc);
				fprintf(output_file, "(Remaining CPU BURST = %d)\n", proc[curr_proc].remaining_t);
				fprintf(output_file, "Ready queue: ");
				printQ(output_file, queue);
				fprintf(output_file,"\n");
			}
			proc[curr_proc].remaining_t--;//decrement remaining burst time
		}

		t++;
	}

	for(i = 0; i < num_of_procs; i++){//calculation of turnaroundtime, wait time, and their averages
		proc[i].tt = proc[i].end_t - proc[i].arrival_t;
		proc[i].wt = proc[i].tt - proc[i].burst_t;
		algorithm_summary -> avg_tt += proc[i].tt;//sum of all tt
		algorithm_summary -> avg_wt += proc[i].wt;//sum of all wt
	}
	algorithm_summary -> avg_tt/= num_of_procs;
	algorithm_summary -> avg_wt /= num_of_procs;

	destroy_Q(queue);//need to destroy the queue after completion of each algorithm

	return algorithm_summary;
}

void algorithm_results(FILE *output_file, PROC_INFO *proc, int num_of_procs, SUMMARY **algorithm_summaries, int algorithm){//print algorithm result based on the value of algorithm

	int i = 0;

	fprintf(output_file, "\n********************************************************\n");

	if(algorithm == 0)
		fprintf(output_file, "FCFS Summary(WT = Wait Time, TT = Turnaround Time)\n\n");
	if(algorithm == 1)
		fprintf(output_file, "SJF Summary(WT = Wait Time, TT = Turnaround Time)\n\n");
	if(algorithm == 2)
		fprintf(output_file, "STCF Summary(WT = Wait Time, TT = Turnaround Time)\n\n");
	if(algorithm == 3)
		fprintf(output_file, "RR Summary(WT = Wait Time, TT = Turnaround Time)\n\n");
	if(algorithm == 4)
		fprintf(output_file, "NPP Summary(WT = Wait Time, TT = Turnaround Time)\n\n");

	fprintf(output_file, "PID\tWT\tTT\n");
	
	while(i < num_of_procs){
		fprintf(output_file, "%d\t%d\t%d\n", i, proc[i].wt, proc[i].tt);
		i++;
	}

	fprintf(output_file, "AVG\t%4.2f\t%4.2f\n\n", algorithm_summaries[algorithm] -> avg_wt, algorithm_summaries[algorithm] -> avg_tt);
	fprintf(output_file, "Process Sequence: ");
	printLL(output_file, algorithm_summaries[algorithm] -> proc_seq);
	fprintf(output_file, "Context Switches: %d\n\n\n", algorithm_summaries[algorithm] -> context_switches);
}

void print_simulation_results(FILE *output_file, SUMMARY **algorithm_summaries){//print the overall simulation results

	int wait_t[5] = {0,1,2,3,4};//corresponding process ids
	int turnaround_t[5] = {0,1,2,3,4};
	int context_switch[5] = {0,1,2,3,4};
	int holder;
	int i,j;
	
	for(i = 0; i < 4; i++){//bubble sort to sort the average wt, tt, and cs
		for(j = 0; j < 4; j++){
			if(algorithm_summaries[wait_t[j]] -> avg_wt > algorithm_summaries[wait_t[j+1]] -> avg_wt){
				holder = wait_t[j];
				wait_t[j] = wait_t[j+1];
				wait_t[j+1] = holder;
			}
			if(algorithm_summaries[turnaround_t[j]] -> avg_tt > algorithm_summaries[turnaround_t[j+1]] -> avg_tt){
				holder = turnaround_t[j];
				turnaround_t[j] = turnaround_t[j+1];
				turnaround_t[j+1] = holder;
			}
			if(algorithm_summaries[context_switch[j]] -> context_switches > algorithm_summaries[context_switch[j+1]] -> context_switches){
				holder = context_switch[j];
				context_switch[j] = context_switch[j+1];
				context_switch[j+1] = holder;
			}
		}
	}

	fprintf(output_file, "***** OVERALL SUMMARY *****\n\n");
	fprintf(output_file, "Wait Time COmparison\n");

	for(i = 0; i < 4; i++){
		if(wait_t[i] == 0)
			fprintf(output_file, "FCFS\t\t%4.2f\n", algorithm_summaries[0] -> avg_wt);
		if(wait_t[i] == 1)
			fprintf(output_file, "SJF\t\t%4.2f\n", algorithm_summaries[1] -> avg_wt);
		if(wait_t[i] == 2)
			fprintf(output_file, "STCF\t\t%4.2f\n", algorithm_summaries[2] -> avg_wt);
		if(wait_t[i] == 3)
			fprintf(output_file, "RR\t\t%4.2f\n", algorithm_summaries[3] -> avg_wt);
		if(wait_t[i] == 4)
			fprintf(output_file, "NPP\t\t%4.2f\n", algorithm_summaries[4] -> avg_wt);
	}

	fprintf(output_file,"\nTurnaround Time Comparison\n");

	for(i = 0; i < 5; i++){
		if(turnaround_t[i] == 0)
			fprintf(output_file, "FCFS\t\t%4.2f\n", algorithm_summaries[0] -> avg_tt);
		if(turnaround_t[i] == 1)
			fprintf(output_file, "SJF\t\t%4.2f\n", algorithm_summaries[1] -> avg_tt);
		if(turnaround_t[i] == 2)
			fprintf(output_file, "STCF\t\t%4.2f\n", algorithm_summaries[2] -> avg_tt);
		if(turnaround_t[i] == 3)
			fprintf(output_file, "RR\t\t%4.2f\n", algorithm_summaries[3] -> avg_tt);
		if(turnaround_t[i] == 4)
			fprintf(output_file, "NPP\t\t%4.2f\n", algorithm_summaries[4] -> avg_tt);
	}

	fprintf(output_file, "\nContext Switch Comparison\n");

	for(i = 0; i < 5; i++){
		if(context_switch[i] == 0)
			fprintf(output_file, "FCFS\t\t%d\n", algorithm_summaries[0] -> context_switches);
		if(context_switch[i] == 1)
			fprintf(output_file, "SJF\t\t%d\n", algorithm_summaries[1] -> context_switches);
		if(context_switch[i] == 2)
			fprintf(output_file, "STCF\t\t%d\n", algorithm_summaries[2] -> context_switches);
		if(context_switch[i] == 3)
			fprintf(output_file, "RR\t\t%d\n", algorithm_summaries[3] -> context_switches);
		if(context_switch[i] == 4)
			fprintf(output_file, "NPP\t\t%d\n", algorithm_summaries[4] -> context_switches);
	}
}











