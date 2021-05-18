#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "input_header.h"
#include "process_header.h"
struct process p[MAX];
int no_of_operations=0;
int total_process;
int main(int argc,char* argv[])
{
	FILE* fp;
	fp = fopen("file.txt","r");
	input_communication(fp);
	
	return 0;
}
void input_communication(FILE* fp)
{
	int i=0;
	if( fp == NULL)
	{
		fprintf(stderr,"Error: Could not open file!!\n");
		exit(0);
	}
	while(!feof(fp))
	{
		fscanf(fp, "%s %[^\n]%*c", p[i].operation, p[i].command);
		printf("%s %s\n", p[i].operation, p[i].command);
		i++;
	}
	no_of_operations=i;
	printf("%d operations read from file.\n",no_of_operations);
	process_main();
}