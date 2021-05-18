#ifndef PROCESS_HEADER_H
#define PROCESS_HEADER_H
#define OPSIZE 500  
#define COMMANDSIZE 500
#define RESPONSESIZE 500
#define MAXPROCESS 50
#define ACK "Recieved"

extern struct entry* process_table;
struct entry
{
	pid_t pid;
	int processID;
	int status;
	int init_time; /*Time when executed*/
	int active_time; /*Time being active*/
	char* process;
};

extern int total_process;

extern int finished;

/* signal handler */

void parse_operation(char*,char*);


void find_status(int,int,char*);
void submit(char* process);
void status(int processID,char* response);
void find_status(int processID,int i,char* response);
void process_main();
char** prepare_for_exec(char* process);
void status_all(int limit,char* response);
int show_finished();
void print_info(int pid);



#endif