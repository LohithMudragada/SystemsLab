#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 

int main(){
	int status = 0;
	char hostname[100], *buf;
	gethostname(hostname,100);
	int pid=0;
	
	do{
			pid_t pid = fork();  
		  char * parsed[] = {"ls","-a","-a"};
			if (pid == -1) { 
				printf("\nFailed forking child.."); 
				//return; 
			} else if (pid == 0) { 
				if (execv("myls", parsed) < 0) { 
				    printf("\nCould not execute command.."); 
				} 
				exit(0); 
			} else { 
				// waiting for child to terminate 
				wait(NULL);  
				//return 0; 
			} 
	}while(status);
	return 0;
}

