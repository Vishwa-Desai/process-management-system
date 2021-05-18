#ifndef INPUT_HEADER_H
#define INPUT_HEADER_H
#define	READ	0      
#define	WRITE	1    
#define length 100
#define MAX 100

struct process{
	char operation[length];
	char command[length];
};
extern struct process p[MAX];


extern int no_of_operations;
void input_communication(FILE*);
void process_main();

#endif