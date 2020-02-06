#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

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
			rid = i;
			break;
		}
	}
	int k=5;

	while(k--){
		if(pid == 0){
			close(pipes[rid][0][1]);
			read(pipes[rid][0][0],id,sizeof(int));
			if(*id == rid) {
				close(pipes[rid][1][0]);
				write(pipes[rid][1][1],id,sizeof(int));
			}
		}
		else{
			printf("%d\n",k);
			for(int i=0 ; i<N ; i++){
				close(pipes[i][0][0]);
				write(pipes[i][0][1],&i,sizeof(int));
				close(pipes[i][1][1]);
				read(pipes[i][1][0],id,sizeof(int));
				printf("responded child : %d\n",*id);
			}
		}
	}
	if(pid == 0){
		close(pipes[rid][0][1]);
		read(pipes[rid][0][0],id,sizeof(int));
		if(*id == rid) printf("Im child : %d\n",rid);
		if(*id == -1)exit(2);
	}
	else{
		//printf("%d\n",k);
		for(int i=0 ; i<N ; i++){
			k=-1;
			close(pipes[i][0][0]);
			write(pipes[i][0][1],&k,sizeof(int));
			close(pipes[i][0][1]);
		}
		while ((wait(&status)) > 0);
	}

	return 0;
}
