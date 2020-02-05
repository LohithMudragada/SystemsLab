#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>


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
        fputc(c);
        c = fgetc(fin);
    }

    return 1;
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

int main(){



    return 0;
}