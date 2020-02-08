// Implementation of sort function

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void bubbleSort(char ** buffer, int low, int high) {
    int i, j;
    for(j = high; j > low; j--) {
        for(i = low; i < j; i++) {
            int cmp_result = strcasecmp(buffer[i], buffer[i+1]);
            if(cmp_result > 0) {
                char * temp = buffer[i];
                buffer[i] = buffer[i+1];
                buffer[i+1] = temp;
            }
        }
    }
}

void sort_c(int count, char *args[]){
    if(count == 3){
        if(strcmp(args[1],"-r") == 0) sort(args[2],0);
        else printf("Enter valid arguments");
    }
    else{
        if(count == 2) sort(args[1],1);
    }
}

void sort(char *fn, int flag) {

    FILE * fp;
    char * line = NULL;
    size_t len = 0,read;
    int numOfLine = 0;
    
    fp = fopen(fn,"r");
    if (fp == NULL)
        exit(1);

    // read number of lines from the file
    while ((read = getline(&line, &len, fp)) != -1) {
        numOfLine++;
    }
    fclose(fp);

    // store the lines of the file in the RAM(created data structure)
    fp = fopen(fn,"r");
    if (fp == NULL)
        exit(1);

    int count = 0;
    char ** line_buffer = (char **) malloc(sizeof(char *) * numOfLine); 
    
    while ((read = getline(&line, &len, fp)) != -1) {
        line_buffer[count] = (char *) malloc(sizeof(char)*read);
        strcpy(line_buffer[count], line);
        if(line_buffer[count][read-1] == '\n')
            line_buffer[count][read-1] = '\0'; // To remove '\n' from the end of line
        count++;
    }

    fclose(fp);
    printf("Number of Lines: %d\n", numOfLine);


    // // Display the lines
    // for(int i = 0; i < numOfLine; i++)
    //     printf("%s\n",line_buffer[i]);

    // Lines after sort
    bubbleSort(line_buffer, 0, numOfLine-1);

    if(flag){
        // Display the lines
        //printf("File After sort: \n");
        for(int i = 0; i < numOfLine; i++)
            printf("%s\n",line_buffer[i]);

    }
   
    else{
        // Display the lines in reverse order
        //printf("File After sort in reverse: \n");
        for(int i = 0; i < numOfLine; i++)
            printf("%s\n",line_buffer[numOfLine - i -1]);

    }
    
}

int main(int count, char *args[]) {
    sort_c(count,args);
    return 0;
}