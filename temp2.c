#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h>
#include <time.h>
#include <sys/stat.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>
#include<dirent.h>

int ls(int count,char *args[])
{
	struct dirent *dp;
	DIR *dir = opendir(".");
    if(count == 1)
        if(dir == NULL) printf("Directory cannot be Opened");
	    else {
			while((dp = readdir(dir)) != NULL) {
                if(dp->d_name[0] != '.')printf("%s\t",dp->d_name);
			}
		}
            
    else{
		if(strcmp(args[1],"-a") == 0) {
			if(dir == NULL) printf("Directory cannot be Opened");
			else while((dp = readdir(dir)) != NULL) printf("%s\t",dp->d_name);
		}
		    	
	}
}

int cp(char *in, char *out){

    FILE *fin, *fout;
    if((fin = fopen(in,"r")) == NULL) {
        printf("can't open input file");
        return -1;
    }
    if((fout = fopen(out,"w")) == NULL) {
        printf("can't open output file");
        return -1;
    }
    char c = fgetc(fin);
    while(c != EOF){
        fputc(c,fout);
        c = fgetc(fin);
    }

    fclose(fin);
    fclose(fout);
    return 1;
}

void cd(char *dir){
    if(dir == NULL) chdir(getenv("home"));
    chdir(dir);
}

void mkdir_commnd(int count, char *args[]){
    printf("In mkdir : %d",count);
    if(count == 2) mkdir(args[1],"700");
    else mkdir(args[1],args[2]);
}

void cat(int count, char *args[]){
    printf("count : %d\n",count);
    if(count < 2) {
        printf("Please Enter Filename\n");
        return;
    }
    if(strcmp(args[1],"-n") == 0){
        FILE *fp = fopen(args[2],"r");
        if(fp == NULL) printf("cant");
        else{
            int i=1;
            printf("%d : ",i);
            char c=fgetc(fp);
            while(c != EOF){
                if(c=='\n') {i++;printf("%c%d : ",c,i);}
                else printf("%c",c);
                c = fgetc(fp);
            }
        }
        
    }
    else{
        FILE *fp = fopen(args[1],"r");
        if(fp == NULL) printf("cant");
        else{
            char c=fgetc(fp);
            while(c != EOF){
                printf("%c",c);
                c = fgetc(fp);
            }
        }
    }
    
}

int grep(char *pat, char *fn){
    printf("%s\t%s\n",pat,fn);
    char line[500];
    int i=1;
    FILE *fp = fopen(fn,"r");
    while(fscanf(fp,"%s\n",line) != EOF){
        if(strstr(line,pat)){
            printf("%d:%s\n",i,line);
        }
        i++;
    }
    fclose(fp);


}

void cp_u(char *in, char *out){
    struct stat tin,tout;
    stat(in, &tin);
    stat(out,&tout);
    if(strcmp(ctime(&tin.st_mtime),ctime(&tout.st_mtime)) !=0 ) cp(in,out);
    else printf("File isn't modified");
}

int input(char *str)
{
    char* buf;
    char host[100],*user,prompt[1024]="\n";
    gethostname(host,100);
    user = getenv("USER");
    strcat(prompt,user);
    strcat(prompt,"@");
    strcat(prompt,host);
    getcwd(host,100);
    strcat(prompt,":~");
    strcat(prompt,host);
    strcat(prompt,"$ ");
    buf = readline(prompt); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
}

int parse(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < 100; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    }
    return i; 
} 



int main(){
    int status = 1;
    do{
        char cmds[1000], *args[10];
        if(input(cmds)) continue;
        int count = parse(cmds,args);

        if(strcmp(args[0],"ls") == 0) ls(count, args);
        if(strcmp(args[0],"cd") == 0) cd(args[1]);
        if(strcmp(args[0],"cat") == 0) cat(count, args);
        if(strcmp(args[0],"mkdir") == 0) mkdir_commnd(count, args);
        if(strcmp(args[0],"cp") == 0){
            if(strcmp(args[1],"-u")==0) cp_u(args[2],args[3]);
            else
            {
                cp(args[1],args[2]);
            }
            
        }
        if(strcmp(args[0],"grep") == 0) {if(strcmp(args[1],"-n")==0)grep(args[2],args[3]);}

        if(strcmp(args[0],"exit") == 0) status = 0;
    
    }
    while(status);
    return 0;
}