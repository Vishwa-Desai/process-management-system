#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /*fork()*/
#include <sys/types.h> /*kill()*/
#include<sys/wait.h>
#include <signal.h>
#include "input_header.h"
#include "process_header.h"
pid_t getpid(void);
int id=1000;
int count=20;
int total_process=0;
int fd[2];
const char* explain_execvp(const char* pathname, char* const* argv);
char** prepare_for_exec(char* process);
void submit(char* process)
{
	int i;
	int place;
	pid_t pid;
	pipe(fd);
	char* command= malloc(RESPONSESIZE * sizeof(char));
	//char* response = malloc(RESPONSESIZE * sizeof(char));
	for(i=0;i<MAXPROCESS;++i)
	{
		if(!process_table[i].processID)
		{
			place = i; //where process_id=0 means that place is empty.
			break;
		}
	}

	process_table[place].status = 1;
	process_table[place].processID = id;
	id++;
	process_table[place].init_time = time(NULL);
	process_table[place].active_time = 0;

	process_table[place].process = malloc((strlen(process)+1)*sizeof(char));
	strcpy(process_table[place].process,process);

	total_process++;
	pid = fork();
	if (pid==0)
	{
		char** arguments = malloc(count * sizeof(char*));
		char* token = malloc(20 * sizeof(char));
		int i;
		
		for (i = 0; i < count; ++i)
		{
			arguments[i] = malloc(20 * sizeof(char));
		}
		i = 1;
		token = strtok(process, " ");
		strcpy(arguments[0], token);
		while (token)
		{
			token = strtok(NULL, " ");
			if (token)
			strcpy(arguments[i], token);
			++i;
		}
		arguments[i-1] = NULL;
		sscanf(process, "%s", command);
		/*for (k = 0; k< 2; k++)
		{
			printf("%s", arguments[k]);
		}*/
		//Execute process
	
			if (execvp(command, arguments) == -1) //job : command that want to run. and arguments means output according to that command.
			{
				printf("Process: %s failed\n", process);
				exit(EXIT_FAILURE);
			}
			//printf("hii");	
	}
	else
	{
		int a;
		sleep(rand()%5);
		a = rand() % 10;
		kill(pid, SIGSTOP);
		sleep(a);
		kill(pid, SIGCONT);
		sleep(rand() % 3);
		process_table[place].active_time = time(NULL) - process_table[place].init_time-a;
		kill(pid, SIGKILL);
		process_table[place].pid = pid;
	}
}

char** prepare_for_exec(char* process)
{
	 char** arguments = malloc(count*sizeof(char*));
	 char* token = malloc(20 * sizeof(char));
	 int i,k;
	 printf("hii_");
	 for(i=0;i<count;++i)
	 {
		arguments[i] = malloc(20*sizeof(char));
	 }
	 i=1;
	 token = strtok(process, " ");
	 strcpy(arguments[0], token);
	while (token)
	{
		token = strtok(NULL, " ");
		if(token)
			strcpy(arguments[i],token);
		++i;
	}
	arguments[i-1]=NULL;
	for (k = 0; k < 2; k++)
	{
		printf("%s\t", arguments[k]);
	}
	return arguments;
}
	
	 
void status(int processID,char* response)
{
	int i;
	/*Coord has already made sure that the given jobID exists*/
	/*Search for this job according to the given jobID*/
	for(i=0;i<total_process;++i)
	{
		if(process_table[i].processID == processID)
		{
			find_status(processID,i,response);
			break;
		}
	}
}

void find_status(int processID,int i,char* response)
{
	switch(process_table[i].status)
	{
		/* Currently Active */
		case 1:
		{
			//int init    = process_table[i].init_time;
			int active  = process_table[i].active_time;
			//int current = time(NULL);


			/* It has never been suspended */
			/*if(!plus)
			{
				running = current - init;
			}
			else
			{
				running = active + current - plus;
			}*/
			sprintf(response,"ProcessID %d Status: Finished (running for %d seconds)",processID,active);
			process_table[i].active_time=active;
			break;
		}
		/* Finished */
		case 2:
		{
			sprintf(response,"ProcessID %d Status: Finished",processID);
			break;
		}
		/* Currently Suspended */
		default:
		{
			sprintf(response,"processID %d Status: Suspended",processID);
		}
	}

}

void status_all(int limit,char *response)
{
	int i;
	char* curr_status = malloc(COMMANDSIZE*sizeof(char));
	int c             = 0;
	int found         = 0;

	for(i=0;i<total_process;++i)
	{

		if(process_table[i].processID)
			if((limit>0 && process_table[i].active_time - process_table[i].init_time <= limit) || (limit==-1))
			{
				found = 1;
				find_status(process_table[i].processID,i,curr_status);
				strcat(curr_status,"\n");
				if(!c)strcpy(response,curr_status);
				else strcat(response,curr_status);
				++c;
			}
	}
	if(!found)strcpy(response,"ZERO PROCESSES FOUND");
	free(curr_status);
}

int show_finished()
{
	int i;
	int c = 0;
	for(i=0;i<total_process;++i)
	{
		if(process_table[i].status == 1)
		{
			printf("processID %d\n",process_table[i].processID);
			c++;
		}
	}
	return c;
}

/*int show_suspended()
{
	int i;
	int c = 0;
	for(i=0;i<total_process;++i)
	{
		if(process_table[i].status == 2)
		{
			printf("processID %d\n",process_table[i].processID);
			c++;
		}
	}
	return c;
}
*/
/*void suspend(int processID)
{
	int i;
	int return_val;

	for(i=0;i<total_process;++i)
	{
		if(process_table[i].processID == processID)
		{
			Send SIGSTOP to this pid 
			//return_val=kill(process_table[i].pid,SIGSTOP);
			if (process_table[i].status == 2)
			{
				int current = time(NULL);
				if (!process_table[i].last_suspended)
				{
					process_table[i].active_time = current - process_table[i].init_time;
					process_table[i].last_suspended = current;
				}
				else
				{
					process_table[i].active_time = current - process_table[i].last_suspended;
					process_table[i].last_suspended = current;
				}
				break;
			}
		}
	}
}	*/		
 
void print_info(int id)
{
	int i;
	for (i = 0; i < total_process; i++)
	{
		if (id == process_table[i].processID)
		{
			printf("process Id : %d\n", process_table[i].processID);
			printf("p Id : %d\n",(int) process_table[i].pid);
			printf("status : %d\n", process_table[i].status);
			printf("init time : %d\n", process_table[i].init_time);
			printf("Active time : %d\n", process_table[i].active_time);
			printf("process name : %s\n", process_table[i].process);
		}
	}

}