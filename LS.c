/*********************** Program To Execute ls command ***********************/
#include<stdio.h>
#include<dirent.h>
#include<string.h>

int main(int count,char *args[])
{
	struct dirent *dp;
	DIR *dir = opendir(".");
		printf("count : %d\n", count);
	if(strcmp(args[2],"-a") == 0) 
		if(dir == NULL) printf("Directory cannot be Opened");
		else while((dp = readdir(dir)) != NULL) printf("%s\t",dp->d_name);
	return 0;
}
