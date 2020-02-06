#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>

// void ls(int count, char * args[]){

//     struct dirent *de;
//     if(count)

// }

int cp(char *in, char *out){

    FILE *fin, *fout;
    if((fin = fopen(in,"r")) == NULL) {
        printf("can't open input file");
        return -1;
    }
    if((fout = fopen(out,"r")) == NULL) {
        printf("can't open output file");
        return -1;
    }
    char c = fgetc(fin);
    while(c != EOF){
        fputc(c,fout);
        c = fgetc(fin);
    }

    return 1;
}

void cd(char *dir){
    if(dir == NULL) chdir(getenv("home"));
    chdir(dir);
}

void mkdir_commnd(char *dn, char *opt){
    if(opt == NULL) mkdir(dn);
    else mkdir(dn,opt);
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
    char array[5000];
    int flag = 1;
    FILE *fp = fopen(fn,"r");
    if(fp == NULL){printf("can't open file");return -1;}
    int ln = 0,count=0;
    char c = fgetc(fp);
    while(c != EOF){
        while(c != '\n' && c != EOF) {
            array[count++] = c;
            c = fgetc(fp);
        }
        array[count++] = '\0';
        ln++;
        for(int j=0 ; j<count ; j++){
            flag = 1;
            int k=0;
            while(pat[k] != '\0') {
                if(pat[k] != array[j+k]) {
                    flag = 0;
                    break;
                }
                k++;
            }
            if(flag) printf("%d:",ln);
        }
    }

}

int input(char *str){
    char* buf; 
  
    buf = readline("\n>>> "); 
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

        if(strcmp(args[0],"cat") == 0) cat(count, args);
        if(strcmp(args[0],"exit") == 0) status = 0;
        if(strcmp(args[0],"cd") == 0) cd(args[1]);
    
    }
    while(status);
    return 0;
}