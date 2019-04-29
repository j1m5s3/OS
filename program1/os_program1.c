/*
 * NAME: James Lynch
 * ID: 01408877
 * DATE: 2/6/19
 * PROFESSOR: Dr. Geiger
 * CLASS: EECE 4810
 * DESCRIPTION: This program utilizes the fork(), wait(), and exec() system calls
 * to show how child processes can run external programs after being created by the parent process.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){

	int num_proc = atoi(argv[1]);//number of processes determined by command line arg
	int proc_status; 	
	int proc_arr[num_proc];	//Array of child process IDs
	int i, argv_index;	//argv_index is used to select a test program

	pid_t proc_id;
	
	printf("Parent ID:  %d\n", getpid());//Display parent ID
	argv_index = 1;
	
	for(i = 0; i < num_proc; i++){
		if(proc_id > 0){	//Parent process only
			proc_id = fork();	//Create a new process
			proc_arr[i] = proc_id;	//Store the child ID 
			argv_index += 1;
			if(argv_index > 6){	
				argv_index = 2;
			}
			if(proc_id < 0){	
				printf("ERROR: FAILED TO CREATE CHILD PROCESS\n");
				return -1;
			}
			else if(proc_id == 0){	//Child process only
				printf("Starting child %d (ID: %d) Parent ID: %d\n", i+1, getpid(), getppid());//print child and parent ids 
				sleep(10);
				execvp(argv[argv_index], argv);	//Child executes specified test program
				exit(0);	//Finish child process execution
			}

			sleep(1);
		}
	}

	if(proc_id > 0){	//Parent process only
		sleep(10);	//Sleep for child processes to execute 
		while(i > 0){
			printf("Child %d (ID: %d) FINISHED\n", (num_proc-i) + 1, waitpid(proc_arr[num_proc-i], &proc_status, WNOHANG));//wait for child processes to terminate
			i--;
		}
		printf("Parent terminating...\n");
		return 0;
	}
}
			

	





