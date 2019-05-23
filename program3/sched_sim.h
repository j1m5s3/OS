#ifndef SCHED_SIM_H_
#define SCHED_SIM_H_

int num_of_procs;
int *arr;
int update_interval;

FILE *input_file;
FILE *output_file;

typedef struct proc_info{
	int cpu_burst;
	int remaining_burst;
	int priority;
	int arrival_time;
	int turnaround_time;
	int wait_time;	
	int ID;
	char status; //New, Primed, Waiting, Running, Completed (N,P,W,R,C)
	int start_t;
	int last_t;
	int end_t;
	int remaining_t;
} PROC_INFO;

PROC_INFO *proc_array;

typedef struct proc_queue{
	queue_node *head;
	queue_node *tail;
}QUEUE;

QUEUE *queue;

typedef struct queue_node{
	PROC_INFO *info;
	queue_node *next;
}QUEUE_NODE;

typedef struct llist_node{
	int data;
	llist_node *next;
}LLIST_NODE;

typedef struct llist{
	LLIST_NODE *head;
	LLIST_NODE *tail;
}LLIST;

LLIST *ll;

typedef struct summarry{
	double avg_wt;
	double avg_tt;
	LLIST *proc_seq;
	int context_switches;
}SUMMARY;

SUMMARY *summary;

void fcfs(int update_interval);
void enqueue(PROC_INFO *proc_array);
int dequeue();
void print_queue();
void initialize();

#endif
