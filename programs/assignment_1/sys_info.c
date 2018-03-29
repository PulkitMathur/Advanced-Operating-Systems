#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int fd[2]; // File Descriptor
	char a1[256];
    	char a2[256];
	
	pipe(fd);
	int status = fork();
	
	if(status == 0)
	{
		/*Child Process*/
		printf("Child PID = %d\n", getpid());
		close(fd[1]);
		read(fd[0],a2,sizeof(a2));
	    
		char *b = a2;
		
		if (strcmp(a2, "/bin/echo") != 0)
		{
			/*non - echo command called*/
			int exeSuccess = execl(a2, b, 0,NULL);
			if(exeSuccess == -1)
			{
				printf("Incorrect Input. Please enter correct input.\n");		
			}
        	} 
        	else 
        	{
			/*echo command called*/
			execl("/bin/echo", "echo", "Hello","World!", NULL);
        	}
        
		
		close(fd[0]);
		exit(0);      
  
	}
	else if(status > 0)
	{
	  /*Parent Process*/
	  printf("Parent PID = %d\n",getpid());
	  close(fd[0]);
	  strcpy(a1, argv[1]);	  
	  write(fd[1],a1,(strlen(a1)+1));
	  close(fd[1]);
	  //wait(NULL);
	  exit(0);
	}
	
	else
	{
		/*Error forking processes*/
		printf("PID = %d\n", getpid());
		fprintf(stderr, "Fork Failed");
		exit(-1);
	}
	
	return 0;
}
