#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(int count,char *args[]){
	

	int pid=0,rid,*id,status = 0;
	int S = atoi(args[1]);
	int N = atoi(args[2]);
	char * fn = args[3];
	printf("%d\t%d\t%s\n",S,N,fn);
	int Game[50],pipes[N][2][2];
	for(int i=0 ; i<N ; i++){
		/*Pipe To Request child a number*/
		pipe(pipes[i][0]);
		/*Pipe To Respond with Number*/
		pipe(pipes[i][1]);
	}

	for(int i=0 ; i<N ; i++){
		if((pid = fork()) == 0){
			close(pipes[i][0][1]); //close the write end of request pipe in child process
			close(pipes[i][1][0]); //close read end of response pipe
			rid = i;//id to communicate with parent and access pipes
			break;
		}
		close(pipes[i][0][0]); //close the read end of request pipe
		close(pipes[i][1][1]); //close the write end of response pipe
	}
	int k=5; // To make temporary loop

	while(k--){

		if(pid == 0){ //If process is child
			
			read(pipes[rid][0][0],id,sizeof(int)); //read if parent written something or not
			if(*id == rid) { // check if parent written or not
				srand(time(0)+getpid());
				int num = 0;
				num = rand()%6 + 1;
				printf("random number : %d\n",num);
				write(pipes[rid][1][1],&num,sizeof(int));
			}
		}
		else{
			printf("%d\n",k);
			for(int i=0 ; i<N ; i++){
				write(pipes[i][0][1],&i,sizeof(int));
				read(pipes[i][1][0],id,sizeof(int));
				printf("responded child : %d\n",*id);
			}
		}
	}

	
	if(pid == 0){
		read(pipes[rid][0][0],id,sizeof(int));
		if(*id == rid) printf("Im child : %d\n",rid);
		close(pipes[rid][1][1]);
		if(*id == -1)exit(0);
	}
	else{
		for(int i=0 ; i<N ; i++){
			k=-1;
			write(pipes[i][0][1],&k,sizeof(int));
			close(pipes[i][0][1]);
		}
		while ((pid = wait(&status)) > 0) printf("Child Exited : %d\n",pid);
	}

	return 0;
}
