/*********************** Program To Execute ls command ***********************/
#include<stdio.h>
#include<dirent.h>
#include<string.h>

int main(int count,char *args[])
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
	return 0;
}
