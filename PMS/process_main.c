#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_header.h"
#include "process_header.h"
int no_of_operations;
struct entry* process_table; //pointer object of process_table structure
struct process p[MAX]; // array of process structure 
void parse_operation(char* operation,char* command);
void process_main()
{
	int ch;
	int i,processID;
	int choice=1;
	int result,limit;
	int id;
	char* operation = malloc(OPSIZE * sizeof(char));
	char* pr = malloc(OPSIZE * sizeof(char));
	char* response = malloc(RESPONSESIZE * sizeof(char));
	char* command = malloc(200*sizeof(char));
	
	process_table  = malloc(MAXPROCESS*sizeof(struct entry));
	for(i=0;i<MAXPROCESS;++i) 
		process_table[i].processID = 0;
	for(int i=0;i<no_of_operations;i++)
	{
		strcpy(operation,p[i].operation);
		strcpy(command,p[i].command);
		parse_operation(operation,command);
	}
	printf("Do you want to run command from console ? (1='yes' 0='No'\n");
	scanf("%d",&ch);
	if(ch==1)
	{
		while(choice>0 || choice<10)
		{
		printf("---------------------\n");
		printf("Select an operation :\n");
		printf("---------------------\n");

		printf(
			"1.submit\n"
			"2.status\n"
			"3.status-all\n"
			"4.show-finished\n"
			"5.Print Info\n");

		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("Enter command :");
				while (getchar() != '\n');
				fgets(pr, OPSIZE * sizeof(char), stdin);
				printf("%s", pr);
				submit(pr);
				break;
			case 2:
				printf("Enter job ID\n");
				scanf("%d",&processID);
				status(processID,response);
				printf("%s",response);
				break;
			case 3:
				printf("Enter time : ");
				scanf("%d", &limit);
				status_all(limit,response);
				printf("%s",response);
				break;
			case 4:
				result=show_finished();
				printf("%d activities are just finished\n",result);
				break;
			case 5:
				printf("Enter process Id : ");
				scanf("%d", &id);
				print_info(id);
				break;
			default:
				printf("Error: Your choice was not valid.Try again !\n");
				break;
		}
	  }
	}
}
void parse_operation(char* operation,char* command)
{
	if(!strcmp(operation,"submit"))
	{
		sscanf(operation,"%*s %[^\t\n]s",command);
		submit(command);
	}
}
