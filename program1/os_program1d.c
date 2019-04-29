/*
 * DATE: 10/1/18
 * NAME: James Lynch
 * Desctription: The purpose of this program is to start a number of 
 * child processes specified by a command line argument. The program then 
 * prints the child# and its id as well as the parent id for all of the 
 * child processes.
 *
 * os_program1d complete!
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[]){
	int command = atoi(argv[1]);	// Store command line arg to int variable
	int status;			// Store status of child process termination
	int pid_arr[command];		// Used to store child pids
	int i = 0;			// counter for loop

	pid_t proc_id;			// Store process id of parent process

	proc_id = getpid();		// Get parent process id

	printf("Parent id: %d\n", proc_id);

	for(i; i < command; i++){
		if(proc_id > 0){	// Parent process is the only process that can enter 
			proc_id = fork();// Create child process
			pid_arr[i] = proc_id;// Store id of child process
			if(proc_id < 0){// Will enter if process does not start corrctly
				printf("Error starting child process...\n");
				exit(-1);
			}
			else if(proc_id == 0){// Child process will enter only
				printf("Child %d has started with id: %d and parent id: %d\n", i, getpid(), getppid());
			}
		}

	}
	
	if(proc_id > 0){		// Only parent process can enter
		sleep(10);		// Wait to make sure all specified child process have started correctly
		while(i > 0){		// Will loop until all child processes have ended
			printf("Child %d with id %d has ended\n",command - i, waitpid(pid_arr[command - i], &status, WNOHANG));
			i--;
		}
		printf("Parent program terminating...\n");
		return 0;
	}
	
}











	
