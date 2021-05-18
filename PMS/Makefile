all: PMS
PMS: input_main.o process_main.o process_operations.o
	gcc -Wall input_main.o process_main.o process_operations.o -o PMS
	
input_main.o: input_main.c
	gcc -c -Wall input_main.c
	
process_main.o: process_main.c
	gcc -c -Wall process_main.c
	
process_operations.o: process_operations.c
	gcc -c -Wall process_operations.c
	
clean:
	rm -rf *o PMS
			
