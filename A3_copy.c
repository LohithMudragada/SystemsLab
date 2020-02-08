#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/wait.h>

int main(int count,char *args[]){
	
    /***** Checking whether Enough Arguments are given or not *****/
	if(count < 4){
		printf("Pass Enough arguments..\n");
		return 0;
	}

    int pid=0,rid,*id,status = 0;
	int S = atoi(args[1]); //Size of Board
	int N = atoi(args[2]); //Number of  Players
	char * fn = args[3]; //FileName
    int Game[50];
    int p_s[N],l[S],s[S];
    for(int i=0 ; i<N ; i++) p_s[i] = 0;
    for(int i=0 ; i<S ; i++){
        l[i] = s[i] = 0;
    }

    /******** File Reading ********/
    FILE *fp = fopen(fn,"r");
    int st,e;
    char c;
    if(!fp){ printf("Can't Open File!!\n"); return 1;}
    while(fscanf(fp,"%c\t%d\t%d\n",&c,&st,&e) != EOF){
        if(c == 'L') {
            if(st < e){
                l[st] = e;
            }
            else{
                printf("Invalid File\n");
                return 1;
            }
            
        }
        else if(c == 'S'){
            if(e < st){
                s[st] = e;
            }
            else{
                printf("Invalid File\n");
                return 1;
            }
        }
        else{
            printf("Invalid File\n");
            return 1;
        }
    }

    for(int i=0 ; i<S ; i++){
        if(l[i] != 0)printf("L\t%d\t%d\n",i,l[i]);
        if(s[i] != 0)printf("S\t%d\t%d\n",i,s[i]);
    }


    int pipes[N][2][2];
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
		}else
        {
            close(pipes[i][0][0]); //close the read end of request pipe
            close(pipes[i][1][1]); //close the write end of response pipe   
        }
        

	}
	int run=1;

	while(run){

		if(pid == 0){ //If process is child
			
			read(pipes[rid][0][0],id,sizeof(int)); //read if parent written something or not
			if(*id == rid) { // check if parent written or not
                int temp = rand();
				srand(time(0)+getpid()+temp);
				int num = 0;
				num = rand()%6 + 1;
				write(pipes[rid][1][1],&num,sizeof(int));
			}else if(*id == -1){
                run = 0;
                close(pipes[rid][0][0]);
		        close(pipes[rid][1][1]);
		        exit(0);
            }
		}
		else{
			printf("%d\n",run);
			for(int i=0 ; i<N ; i++){
                do{
                    write(pipes[i][0][1],&i,sizeof(int));
                    read(pipes[i][1][0],id,sizeof(int));
                    int temp = p_s[i]+*id;
                    if(temp > S){
                        printf("Winner is : %d\n",i);
                        run = 0;
                        break;
                    }
                    else{
                        if(l[temp]) p_s[i] = l[temp];
                        else if(s[temp]) p_s[i] = s[temp];
                        else p_s[i] = temp;
                    }
                    printf("player %d is at : %d\n",i,p_s[i]);
                }while(*id == 6);
                if(run == 0) break;

                
			}
		}
	}

	


    for(int i=0 ; i<N ; i++){
        int k=-1;
        write(pipes[i][0][1],&k,sizeof(int));
        close(pipes[i][0][1]);
        close(pipes[i][1][0]);
    }
    while ((pid = wait(&status)) > 0) printf("Child Exited : %d\n",pid);

	return 0;
}
