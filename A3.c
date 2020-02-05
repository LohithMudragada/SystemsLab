#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int count,char *args[]){

	int pid,rid,*id;
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
			rid = i;
			break;
		}
	}

	if(pid == 0){
		close(pipes[rid][0][1]);
		read(pipes[rid][0][0],id,sizeof(int));
		if(*id == rid) printf("Im child : %d\n",rid);
		exit(2);
	}
	else{
		for(int i=0 ; i<N ; i++){
			close(pipes[rid][0][0]);
			write(pipes[i][0][1],&i,sizeof(int));
		}
	}

	return 0;
}
