#ifndef SCHED_SIM_H_
#define SCHED_SIM_H_

int num_of_procs;
int *arr;

typedef struct process_info{
	int cpu_burst;
	int priority;
	int arrival_time;
} proc_info;

proc_info *proc_array;

typedef struct ready_queue{
	int front;
	int rear;
	int size;
	int capacity;
	int *array;
}QUEUE;

QUEUE *queue;



void fcfs(int update_interval);
void enqueue(int arrival_check);
void initialize();

#endif
