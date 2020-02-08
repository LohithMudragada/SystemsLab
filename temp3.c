#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/wait.h>


int readandval(char *fn, int *l, int *s){
    /******** File Reading ********/
    printf("Reading File\n");
    FILE *fp = fopen(fn,"r"); //open file in read mode
    int st,e;
    char c;
    if(!fp){ 
        printf("Can't Open File!!\n"); 
        return 1;
    }
    while(fscanf(fp,"%c\t%d\t%d\n",&c,&st,&e) != EOF){ //scan file line by line
        if(c == 'L') {
            if(st < e){
                if(s[st]){
                    printf("Invalid File\n");
                    return 1;
                }
                l[st] = e;
            }
            else{
                printf("Invalid File\n");
                return 1;
            }
            
        }
        else if(c == 'S'){
            if(e < st){
                if(l[st]){
                    printf("Invalid File\n");
                    return 1;
                }
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
    fclose(fp);
    return 0;

}


void game(int count, char *args[]){

    printf("In Game \n");
    int pid=0,rid,*id;
    int S = atoi(args[1]); //Size of Board
	int N = atoi(args[2]); //Number of  Players
	char * fn = args[3]; //FileName
    int p_s[N],l[S],s[S];
    for(int i=0 ; i<N ; i++) p_s[i] = 0; //initiate all players position to 0
    for(int i=0 ; i<S ; i++){
        l[i] = s[i] = 0; //initiate all snakes and ladders position to 0
    }
    readandval(fn,l,s);

    int pipes[N][2][2];
	for(int i=0 ; i<N ; i++){
		/*Pipe To Request child a number*/
		pipe(pipes[i][0]);
		/*Pipe To Respond with Number*/
		pipe(pipes[i][1]);
	}
    printf("Initiated Pipes\n");

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

    int run = 1;
    printf("Childs Successfully created");
    if( pid == 0) {
        while(run){
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
    }
    else{
        int i = rand()%N;
        while (run)
        {
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
            
            if(i<N-1) i++;
            else i = 0;
        }
        for(int i=0 ; i<N ; i++){
            int k=-1;
            write(pipes[i][0][1],&k,sizeof(int));
            close(pipes[i][0][1]);
            close(pipes[i][1][0]);
        }
        
    }

    while ((pid = wait(NULL)) > 0) printf("Child Exited : %d\n",pid);


}

int main(int count,char *args[]){

    /***** Checking whether Enough Arguments are given or not *****/
	if(count < 4){
		printf("Pass Enough arguments..\n");
		return 0;
	}

    game(count,args);
    

    return 0;
}