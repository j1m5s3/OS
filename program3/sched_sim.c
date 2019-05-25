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

	SUMMARY *algo_summary = (SUMMARY *)malloc(sizeof(SUMMARY));

	algo_summary -> avg_wt = 0;
	algo_summary -> avg_tt = 0;
	algo_summary -> proc_seq = create_LL();
	algo_summary -> context_switches = 0;

	return algo_summary;
}

void destroy_Q(QUEUE* queue){

	NODE *current = queue -> head;

	while(current != NULL){

		current = queue -> head -> next;
		free(queue -> head);
		queue -> head = current;
	}

	free(queue);
	queue = NULL;
}

void destroy_LL(LL *linked_l){

	LL_NODE *current = linked_l -> head;

	while(current != NULL){

		current = linked_l -> head -> next;
		free(linked_l -> head);
		linked_l -> head = current;
	}

	free(linked_l);
	linked_l = NULL;
}

void destroy_SUMMARY(SUMMARY *algorithm_summary){

	destroy_LL(algorithm_summary -> proc_seq);
	free(algorithm_summary);
	algorithm_summary = NULL;
}

void enQ(QUEUE *queue, PROC_INFO *proc){
	

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
	printf("ENQ: %d\n", __LINE__);
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
		while(CURRENTnode -> next != NULL && CURRENTnode -> next -> info -> burst_t < proc -> burst_t){

			CURRENTnode = CURRENTnode -> next;
		}

		if(queue -> tail == CURRENTnode){
			queue -> tail = NEWnode;
		}

		NEWnode -> next = CURRENTnode -> next;
		CURRENTnode -> next = NEWnode;
	}
}

void remainingBURST_enQ(QUEUE *queue, PROC_INFO *proc){

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

	if(queue -> head -> info -> remaining_t > proc -> remaining_t){

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

void priority_enQ(QUEUE *queue, PROC_INFO *proc){

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
		while(CURRENTnode -> next != NULL && CURRENTnode -> next -> info -> priority < proc -> priority){
			
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

int deQ(QUEUE *queue){

	int id;
	NODE *TEMPnode;

	if(Qempty(queue)){
		return -1;
	}
	
	TEMPnode = queue -> head;
	id = TEMPnode -> info -> id;

	queue -> head = queue -> head -> next;

	free(TEMPnode);

	if(queue -> head == NULL){
		queue -> tail = NULL;
	}
	printf("deQ: %d\n", __LINE__);

	return id;
}

int checkQ(QUEUE *queue){
	return queue -> head -> info -> id;
}

int Qempty(QUEUE *queue){
	return queue -> head == NULL && queue -> tail == NULL;
}

void printQ(FILE *output_file, QUEUE *queue){

	NODE CURRENTnode;

	if(Qempty(queue)){
		fprintf(output_file, "EMPTY\n");
		return;
	}

	CURRENTnode.info = queue -> head -> info;
	CURRENTnode.next = queue -> head -> next;

	while(CURRENTnode.next != NULL){

		fprintf(output_file, "%d-", CURRENTnode.info -> id);
		printf("printQ: %d\n", __LINE__);
		CURRENTnode.info = CURRENTnode.next -> info;
		CURRENTnode.next = CURRENTnode.next -> next;
	}
	fprintf(output_file, "%d\n", queue -> tail -> info -> id);
	printf("printQ: %d\n",__LINE__);
}

void add_LLnode(LL *linked_l, int id){

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

void printLL(FILE *output_file,LL *linked_l){

	LL_NODE CURRENTnode;

	if(LLempty(linked_l)){

		fprintf(output_file, "EMPTY\n");
		return;
	}

	CURRENTnode.proc_id = linked_l -> head -> proc_id;
	CURRENTnode.next = linked_l -> head -> next;

	while(CURRENTnode.next != NULL){
		
		fprintf(output_file, "%d-", CURRENTnode.proc_id);
		CURRENTnode.proc_id = CURRENTnode.next -> proc_id;
		CURRENTnode.next = CURRENTnode.next -> next;
	}
	fprintf(output_file, "%d\n", linked_l -> tail -> proc_id);
}

int LLempty(LL *linked_l){
	return linked_l -> head == NULL && linked_l -> tail == NULL;
}

SUMMARY* simulation(FILE *output_file, int update_interval, PROC_INFO *proc, int num_of_procs, int algorithm){

	int i;
	int t = 0;
	int complete = 0;
	int curr_proc = -1;
	printf("SIM %d  %d\n", algorithm, __LINE__);
	SUMMARY *algorithm_summary = create_SUMMARY();
	QUEUE *queue = create_Q();
	
	if(algorithm == 0)
		fprintf(output_file, "***** FCFS Scheduling *****\n");
	if(algorithm == 1)
		fprintf(output_file, "***** SJF Scheduling *****\n");
	if(algorithm == 2)
		fprintf(output_file, "***** STCF Scheduling *****\n");
	if(algorithm == 3)
		fprintf(output_file, "***** STCF Scheduling *****\n");
	if(algorithm == 4)
		fprintf(output_file, "***** STCF Scheduling *****\n");

	while(complete < num_of_procs){
		printf("Complete: %d; t = %d; Active Proc: %d\n", complete, t, curr_proc);
		for(i = 0; i < num_of_procs; i++){
			printf("ID: %d; Arr_t: %d; remain_t: %d\n", proc[i].id,proc[i].arrival_t,proc[i].remaining_t);
			if(t >= proc[i].arrival_t){
				if(proc[i].status == 'N' || proc[i].status == 'W'){
					if(algorithm == 0){//FCFS
						enQ(queue, &proc[i]);
						printf("FCFS: %d\n",__LINE__);
						proc[i].status = 'R';
					}
					if(algorithm == 1){//SJF
						burst_enQ(queue, &proc[i]);
						printf("SJF: %d\n",__LINE__);
						proc[i].status = 'R';
					}
					if(algorithm == 2){//STCF
						remainingBURST_enQ(queue, &proc[i]);
					  	printf("STCF: %d\n",__LINE__);
						proc[i].status = 'R';
					}
					if(algorithm == 3){//RR
						enQ(queue, &proc[i]);
						printf("RR: %d\n",__LINE__);
						proc[i].status = 'R';
					}
					if(algorithm == 4){//NPP
						priority_enQ(queue, &proc[i]);
						printf("NPP: %d\n",__LINE__);
						proc[i].status = 'R';
					}
				}
			}
		}
		if(curr_proc == -1 && !Qempty(queue)){
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
			proc[curr_proc].remaining_t--;
			algorithm_summary -> context_switches++;
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);
			printf("SIM: %d\n",__LINE__);

		}
		else if(curr_proc == -1 && Qempty(queue)){
			if(t % update_interval == 0){
				fprintf(output_file,"t = %d\n", t);
				fprintf(output_file, "CPU: Waiting for processes\n");
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
		}
		else if(proc[curr_proc].remaining_t <= 0 && !Qempty(queue)){
			if(t % update_interval == 0){	
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d; Loading Process %d ", curr_proc, checkQ(queue));
				fprintf(output_file, "(CPU BURST = %d)\n", proc[checkQ(queue)].remaining_t);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file,"\n");
			}

			proc[curr_proc].status = 'C';
			proc[curr_proc].end_t = t;
			complete++;
			curr_proc = deQ(queue);//segfault appears to heappen here
			proc[curr_proc].status = 'A';

			if(proc[curr_proc].remaining_t == proc[curr_proc].burst_t){
				proc[curr_proc].start_t = t;
			}
			proc[curr_proc].last_run = t;
			proc[curr_proc].remaining_t--;
			algorithm_summary -> context_switches++;
			add_LLnode(algorithm_summary -> proc_seq, curr_proc);
			printf("SIM: %d\n",__LINE__);

		}
		else if(proc[curr_proc].remaining_t <= 0 && Qempty(queue)){
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d \n", curr_proc);
				fprintf(output_file, "Ready Queue: ");
				printQ(output_file, queue);
				fprintf(output_file, "\n");
			}
			proc[curr_proc].status = 'C';
			proc[curr_proc].end_t = t;
			complete++;
			curr_proc = -1;
		}
		else if(algorithm == 2 && !Qempty(queue) && proc[curr_proc].remaining_t > proc[checkQ(queue)].remaining_t){
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
		else if(algorithm == 3 && !Qempty(queue) && t - proc[curr_proc].last_run >= QUANTUM){
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Preempting Process %d (Remaining CPU BURST = %d); ", curr_proc, proc[curr_proc].remaining_t);
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
		else{
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Running Process %d ", curr_proc);
				fprintf(output_file, "(Remaining CPU BURST = %d)\n", proc[curr_proc].remaining_t);
				fprintf(output_file, "Ready queue: ");
				printQ(output_file, queue);
				fprintf(output_file,"\n");
			}
			proc[curr_proc].remaining_t--;
		}

		t++;
	}

	for(i = 0; i < num_of_procs; i++){
		proc[i].tt = proc[i].end_t - proc[i].arrival_t;
		proc[i].wt = proc[i].tt - proc[i].burst_t;
		algorithm_summary -> avg_tt += proc[i].tt;
		algorithm_summary -> avg_wt += proc[i].wt;
	}
	algorithm_summary -> avg_tt/= num_of_procs;
	algorithm_summary -> avg_wt /= num_of_procs;

	destroy_Q(queue);

	return algorithm_summary;
}

void algorithm_results(FILE *output_file, PROC_INFO *proc, int num_of_procs, SUMMARY **algorithm_summaries, int algorithm){

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

void print_simulation_results(FILE *output_file, SUMMARY **algorithm_summaries){

	int wait_t[5] = {0,1,2,3,4};
	int turnaround_t[5] = {0,1,2,3,4};
	int context_switch[5] = {0,1,2,3,4};
	int holder;
	int i;
	
	for(i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
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

	for(i = 0; i < 4; i++){
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
	for(i = 0; i < 4; i++){
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











