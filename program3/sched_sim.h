#ifndef SCHED_SIM_H
#define SCHED_SIM_H

extern const int QUANTUM;

typedef struct proc_info{

	int id;//process id
	int burst_t;//process CPU BURST taken from input file
	int priority;//process prioity# taken from input file
	int arrival_t;//process arrival time taken from input file
	int start_t;//track what time t the process starts
	int end_t;//track what time t the process completes
	int last_run;//track what time t the process ran
	int remaining_t;//remaining burst time of the process
	int tt;//turnaround time
	int wt;//wait time
	char status;//New, Ready, Active, Waiting, Completed (N,R,A,W,C)
}PROC_INFO;

//Linked list structs
typedef struct ll_node{

	int proc_id;
	struct ll_node *next;
}LL_NODE;

typedef struct ll{

	LL_NODE *head;
	LL_NODE *tail;
}LL;

//Queue structs
typedef struct node{

	PROC_INFO *info;
	struct node *next;
}NODE;

typedef struct queue{

	NODE *head;
	NODE *tail;
}QUEUE;

//Algorithm summary struct
typedef struct summary{

	double avg_wt;//average wait time for the process
	double avg_tt;//average turnaroundtime for the process
	int context_switches;//count the context switches
	LL *proc_seq;//access to the linked list which holds sequence in which the processes run
}SUMMARY;

QUEUE* create_Q();
LL* create_LL();
SUMMARY* create_SUMMARY();

void destroy_Q(QUEUE *queue);
void destroy_LL(LL *linked_l);
void destroy_SUMMARY(SUMMARY *algo_summary);

void enQ(QUEUE *queue, PROC_INFO *proc);
void burst_enQ(QUEUE *queue, PROC_INFO *proc);
void remainingBURST_enQ(QUEUE *queue, PROC_INFO *proc);
void priority_enQ(QUEUE *queue, PROC_INFO *proc);
int deQ(QUEUE *queue);
int checkQ(QUEUE *queue);
int Qempty(QUEUE *queue);
void printQ(FILE *output_file, QUEUE *queue);

void add_LLnode(LL *linked_l, int id);
void printLL(FILE *output_file, LL *linked_l);
int LLempty(LL *linked_l);

SUMMARY* simulation(FILE *output_file, int update_interval, PROC_INFO *proc, int num_of_procs, int algorithm);//0,1,2,3,4 (FCFS,SJF,STCF,RR,NPP)

void algorithm_results(FILE *output_file, PROC_INFO *proc,int num_of_procs,SUMMARY **algo_summaries, int algorithm);
void print_simulation_results(FILE *output_file, SUMMARY **algo_summaries);

#endif


