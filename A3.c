#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(int count,char *args[]){
	

	int pid=0,rid,*id;
	if(count < 3){
		printf("Pass Enough arguments..\n");
		return;
	}
	int S = atoi(args[1]);
	int N = atoi(args[2]);
	int *p_status = (int *)malloc(N*sizeof(int));
	char * fn = args[3];
	printf("%d\t%d\t%s\n",S,N,fn);
	int pipes[N][2][2];
	for(int i=0 ; i<N ; i++){
		/*Pipe To Request child a number*/
		pipe(pipes[i][0]);
		/*Pipe To Respond with Number*/
		pipe(pipes[i][1]);
	}

	for(int i=0 ; i<N ; i++){
		if((pid = fork()) == 0){
			rid = i;//id to communicate with parent and access pipes
			break;
		}
	}
	int k=5; // To make temporary loop

	while(k--){

		if(pid == 0){ //If process is child
			close(pipes[rid][0][1]); //close the write end of request pipe in child process
			read(pipes[rid][0][0],id,sizeof(int)); //read if parent written something or not
			if(*id == rid) { // check if parent written or not
				close(pipes[rid][1][0]); //close the read end of response pipe
				srand(time(0)+getpid()); //seed to generate random number
				int num = 0;
				num = rand()%6 + 1;
				printf("random number : %d\n",num);
				write(pipes[rid][1][1],&num,sizeof(int)); //write the random number to parent
			}
		}
		else{

			printf("\nIteration : %d\n",k);

			for(int i=0 ; i<N ; i++){
				close(pipes[i][0][0]); //close the read end of request pipe
				write(pipes[i][0][1],&i,sizeof(int)); //write request to child having 'rid' 'i'
				close(pipes[i][1][1]); //close the write end of response pipe
				read(pipes[i][1][0],id,sizeof(int)); //read random number
				printf("****child %d response : %d****\n",i,*id);
			}
		}
	}

	/********** Exit All Processes **********/
	
	if(pid == 0){
		close(pipes[rid][0][1]); //close the write end of request pipe in child process
		read(pipes[rid][0][0],id,sizeof(int)); //read value written by parent
		if(*id == rid) printf("Im child : %d\n",rid);
		if(*id == -1){ // if value is -1
			close(pipes[rid][1][1]); //close write of response pipe
			close(pipes[rid][0][0]); //close read end of request pipe
			exit(0); //exit
		}
	}
	else{
		for(int i=0 ; i<N ; i++){
			k=-1;
			close(pipes[i][0][0]); //close read end of request pipe
			write(pipes[i][0][1],&k,sizeof(int)); //say child to exit
			close(pipes[i][0][1]); //close write end of request pipe
			close(pipes[i][1][0]); //close read end of response pipe
			close(pipes[i][1][1]); //close write end of response pipe
		}
		while ((pid = wait(NULL)) > 0) printf("Child Exited : %d\n", pid);
	}

	return 0;
}
