#include <stdio.h>
#include <stdlib.h>
#include "sched_sim.h"

void initialize(){
	queue = (QUEUE *)malloc(sizeof(QUEUE));
	queue -> head = NULL;
	queue -> tail = NULL;

	ll = (LLIST *)malloc(sizeof(LLIST));
	ll -> head = NULL;
	ll -> tail = NULL;	

	summary = (SUMMARY *)malloc(sizeof(SUMMARY));
	summary -> avg_wt = 0;
	summary -> avg_tt = 0;
	summary -> proc_seq = ll;
	summary -> context_switches = 0;

	proc_array = (PROC_INFO *)malloc(num_of_procs*sizeof(PROC_INFO));
	arr = (int *)malloc(num_of_procs*3*sizeof(int));
}

void fcfs(int update_interval){
	int t = 0;
	int burst;
	int arrival_check = 0;
	int completed = 0;
	int curr_proc = -1;
	QUEUE_NODE current;

	fprintf(output_file, "**** FCFS ****\n");

	while(completed < num_of_procs){//loop for processes
		while(arrival_check < num_of_procs){
			if(proc_array[arrival_check].arrival_time == t && proc_array[arrival_check].status =='N'){
				enqueue(proc_array[arrival_check]);
				proc_array[arrival_check].status = 'P';//Proc is primed and on the queue
			}
			
			arrival_check++;
		}

		arrival_check = 0;

	
		if(queue -> head != queue -> tail && curr_proc == -1){//Loading first process
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Loading process %d ", queue -> head -> info -> ID);
				fprintf(output_file, "(CPU burst = %d)\n", queue -> head -> info -> cpu_burst);

				print_queue();

				fprintf(output_file, "\n");
			}

			curr_proc = dequeue();
			proc_array[curr_proc].status = 'R';
			proc_array[curr_proc].start_t = t;
			proc_array[curr_proc].last_t = t;
			proc_array[curr_proc].remaining_t--;
			summary -> context_switches++;
			ll_add(curr_proc);
		}
		else if(proc_array[curr_proc].remaining_t == 0 && queue -> head != queue -> tail){
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d; ", curr_proc);
				fprintf(output_file, "Loading Process %d ", queue -> head -> info -> ID);
				fprintf(output_file, "(CPU Burst = %d)\n" queue -> head -> info -> cpu_burst);

				print_queue();

				fprintf(output_file, "\n");
			}

			proc_array[curr_proc].status = 'C';
			proc_array[curr_proc].end_t = t;
			complete++;
			curr_proc = dequeue();
			proc_array[curr_proc].status = 'R';
			proc_array[curr_proc].start_t = t;
			proc_array[curr_proc].last_t = t;
			proc_array[curr_proc].remaining_t--;
			summary -> context_switches++;
			ll_add(curr_proc);
		}
		else if(proc_array[curr_proc].reamaining_t == 0 && queue -> head == queue -> tail){
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Finishing Process %d\n", curr_proc);

				print_queue();

				fprintf(output_file, "\n");
			}

			proc_array[curr_proc].status = 'C';
			proc_array[curr_proc].end_t = t;
			complete++;
		}
		else{ 
			if(t % update_interval == 0){
				fprintf(output_file, "t = %d\n", t);
				fprintf(output_file, "CPU: Running process %d ", curr_proc);
				fprintf(output_file, "(Remaining CPU Burst = %d)\n", proc_array[curr_proc].remaining_t);

				print_queue();

				fprintf(output_file, "\n");
			}

			proc_array[curr_proc].remaining_t--;
		}

		t++;
	}
}

void print_queue(){
	QUEUE_NODE current;

	current.info = head -> info;
	current.info = head -> next;

	fprintf(output_file, "Ready queue: ");

	if(queue -> head == queue -> tail){
		fprintf(output_file,"EMPTY\n");
	}

	while(current.next != NULL){
		fprintf(output_file "%d-", current.info -> ID);
		current.info = current.next -> info;
		current.next = current.next -> next;
	}

	fprintf(output_file, "%d\n", queue -> tail -> info -> ID);
}

void enqueue(PROC_INFO *proc_array){
	 QUEUE_NODE *NEWnode = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
	 NEWnode -> info = proc_array;
	 NEWnode -> next = NULL;

	 if(queue -> tail == NULL){//
		 queue -> head = queue -> tail = NEWnode;
	 }
	 else
	 {
		 queue -> tail -> next = NEWnode;
		 queue -> tail = NEWnode;
	 }
}

int dequeue(){//dequeue a process when it is executing, becomes ACTIVE procecss
	int ID;
	QUEUE_NODE *temp;

	if(queue -> head == NULL){
		return -1;
	}

	temp = queue -> head;
	ID = temp -> info -> ID;
	queue -> head = queue -> head -> next;

	free(temp);

	if(queue -> head == NULL){
		queue -> tail = NULL;
	}

	return ID;
}

void ll_add(int ID){
	LLIST_NODE *NEWnode = (LLIST_NODE *)malloc(sizeof(LLIST_NODE));
	NEWnode -> data = ID;
	NEWnode -> next = NULL;
	
	if(ll -> head = NULL && ll -> tail = NULL){
		ll -> head = NEWnode;
		ll -> tail = NEWnode
	}
	else 
	{
		ll -> tail -> next = NEWnode;
		ll -> tail = NEWnode;
	}
}
