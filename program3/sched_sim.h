#ifndef SCHED_SIM_H
#define SCHED_SIM_H

extern const int QUANTUM;

typedef struct proc_info{

	int id;
	int burst_t;
	int priority;
	int arrival_t;
	int start_t;
	int end_t;
	int last_run;
	int remaining_t;
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

	double avg_wt;
	double avg_tt;
	int context_switches;
	LL *proc_seq;
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


