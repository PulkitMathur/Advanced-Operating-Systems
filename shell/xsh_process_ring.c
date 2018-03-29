
/* xsh_process_ring.c - xsh_process_ring */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <process_ring.h>

#define NUM_PROCESSES 2 
#define NUM_ROUNDS 3 
#define MAX 4000

/*------------------------------------------------------------------------
 * xsh_process_ring 
 *------------------------------------------------------------------------
 */


/*
 *	Function Name: checkArguments
 *	Parameters In: Number of processes and rounds
 *	Parameters Out: int (1/0)
 *	Description: Checks whether the set of input arguments are correct or not.
 */
int checkArguments(int num_proc,int rounds)
{
	if(num_proc <= 0 && rounds <= 0)
	{
		printf("Number of Processes and Number of Rounds should be a Number!!\n");
		return 0;	
	}
			
	if(num_proc <= 0)
	{
		printf("Number of Processes should be a Number!!\n");
		return 0;
	}
			
	if(rounds <= 0)
	{
		printf("Number of Rounds should be a Number!!\n");
		return 0;
	}
	return 1;
}



/*
 *	Function Name: pollingFunction
 *	Parameters In: Number of processes and rounds
 *	Parameters Out: void/none
 *	Description: Creates processes and runs all according to the number of rounds and prints the values.
 */
void pollingFunction(int num_proc, int rounds)
{
	printf("Number of Processes: %d\n", num_proc);
	printf("Number of Rounds: %d\n", rounds);

	int tc = num_proc*rounds-1;
	int arr[MAX];
	arr[0] = tc;
	int cnt = 0;
	int i = 1;
	int lf = 0;

	while(i <= rounds)
	{
		int k = 1;
		while(k<=num_proc)
		{
			if(cnt <= tc)
			{
				resume(create(my_polling_proc, 1024, 20, "my_polling_proc",4, &arr[cnt],&arr[cnt+1], &i, &k));
				cnt++;
				k++;
			}
			else
			{
				lf = 1;
				break;
			}
		}

		i++;
		if(lf == 1)
		{
			break;
		}
	}
}

/*
 *	Function Name: xsh_process_ring
 *	Parameters In: Number of processes and rounds
 *	Parameters Out: shellcmd
 *	Description: Main code for process_ring command.
 */
shellcmd xsh_process_ring(int nargs, char *args[]) 
{	
	/*Checking for then number of inputs*/
	if (nargs < 1) {
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
		return 1;
	}
	
	if (nargs > 8) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
		return 1;
	}
	
	/* Output info for '--help' argument */
	
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tPrints the countdown of numbers\n");
		printf("\tArguments taken from the command line are -p -r -v -h.\n");
		printf("Options (one per invocation):\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	/*Default Case*/
	if (nargs == 1) 
	{
		uint32 start,end;
		int32 retval;
		retval = gettime(&start);
		if (retval == SYSERR){
			return SYSERR;
		}
		pollingFunction(NUM_PROCESSES,NUM_ROUNDS); 
		retval = gettime(&end);
		if (retval == SYSERR){
			return SYSERR;
		}
		int32 sec = end - start;
		printf("\nElapsed Seconds: %d\n",sec);
		return SHELL_OK;
	}
	
	/*Incorrect/Missing arguments case*/
	if (nargs % 2 == 0) 
	{
		fprintf(stderr, "%s: Wrong Set of Inputs for: \n", args[0]);
		fprintf(stderr, "Below find some sample inputs: \n");
		fprintf(stderr, "	1. process_ring 3 4\n");
		fprintf(stderr, "	2. process_ring -r 3 -p 4\n");
		fprintf(stderr, "	3. process_ring -p 64 -r 1 -i poll\n");
		fprintf(stderr, "	4. process_ring -p 1 -r 100 -i sync\n");
		fprintf(stderr, "	5. process_ring --help\n");
		fprintf(stderr, "-p -> Number of Processes\n");
		fprintf(stderr, "-r -> Number of Rounds\n");
		fprintf(stderr, "-i -> Polling/Sync\n");
		return 1;
	}

	
	int num_proc = 0;
	int rounds = 0;
	char type[50];
	uint32 start,end;
	int32 retval,sec;

	//Polling
	int tc = -1;
	int32 arr[MAX];
	arr[0] = tc;
	int cnt = 0;
	int i = 1;
	int lf = 0;
	
	// Sync
	volatile int val[MAX] = {-1};
	volatile int round = 0;
	volatile sid32 semaphoreList[MAX];
	sid32 donesem;

		
	/* Code for 3/5/7 arguments */
	switch(nargs) 
	{
		case 3:
			// Using polling
			num_proc = atoi(args[1]);
			rounds = atoi(args[2]);
			if(checkArguments(num_proc,rounds) == 0)
			{
				return SHELL_ERROR;	
			}
			
			retval = gettime(&start);
			if (retval == SYSERR){
				return SYSERR;
			}
			
			pollingFunction(num_proc,rounds);
			
			retval = gettime(&end);
			if (retval == SYSERR){
				return SYSERR;
			}
			sec = end - start;
			printf("\nElapsed Seconds: %d\n",sec);
			return SHELL_OK;
			break;
		case 5:
			// Using polling
			if(strncmp("-p", args[1], 3) == 0) 
			{
				num_proc = atoi(args[2]);
				rounds = atoi(args[4]);
			}
			else if(strncmp("-r", args[1], 3) == 0)
			{
				num_proc = atoi(args[4]);
				rounds = atoi(args[2]);
			}
			
			if(checkArguments(num_proc,rounds) == 0)
			{
				return SHELL_ERROR;	
			}
			
			retval = gettime(&start);
			if (retval == SYSERR){
				return SYSERR;
			}
			pollingFunction(num_proc,rounds);
			retval = gettime(&end);
			if (retval == SYSERR){
				return SYSERR;
			}
			sec = end - start;
			printf("\nElapsed Seconds: %d\n",sec);
			
			return SHELL_OK;
			break;
		case 7:
			for(i=1;i<nargs;i++)
			{
				if(strncmp("-r", args[i], 3) == 0)
				{
					rounds = atoi(args[i+1]);
				}
				else if(strncmp("-p", args[i], 3) == 0)
				{
					num_proc = atoi(args[i+1]);
				}
				else if(strncmp("-i", args[i], 3) == 0)
				{
					strcpy(type, args[i+1]);
				}
			}
			if(checkArguments(num_proc,rounds) == 0)
			{
				return SHELL_ERROR;	
			}


			printf("Number of Processes: %d\n", num_proc);
			printf("Number of Rounds: %d\n", rounds);
			
			if(strncmp("poll", type, 5) == 0)
			{
				// Using polling
				retval = gettime(&start);
				if (retval == SYSERR){
				return SYSERR;
				}
				
				i = 1;
				tc = num_proc*rounds - 1;
				arr[0] = tc;
				while(i <= rounds)
				{
					int k = 1;
					while(k <= num_proc)
					{
						if(cnt <= tc)
						{
							resume(create(my_polling_proc, 1024, 20, "my_polling_proc",4, &arr[cnt],&arr[cnt+1], &i, &k));
                   			 		cnt++;
                    					k++;
						}
						else
						{
							lf = 1;
							break;
						}
					}

					i++;
					if(lf == 1)
					{
						break;
					}
				}
				
				retval = gettime(&end);
				if (retval == SYSERR){
					return SYSERR;
				}
				sec = end - start;
				printf("\nElapsed Seconds: %d\n",sec);
				
				return SHELL_OK;
			}
			else if(strncmp("sync", type, 5) == 0)
			{
				// Using Semaphore
				retval = gettime(&start);
				if (retval == SYSERR){
					return SYSERR;
				}
				int count = rounds;
				
				val[0] = num_proc*rounds-1;
				
				for(i=1;i<num_proc;i++)
				{
                        		val[i]=2;
        		}
				
				semaphoreList[0] = semcreate(1);
				
				for(i=1; i<num_proc; i++)
				{
					semaphoreList[i] = semcreate(0);
				}
				
				donesem = semcreate(0);
				
				for(i=0 ; i<num_proc; i++)
				{        
                 			resume(create(my_semaphore_proc, 1024, 20, "my_semaphore_proc", 7, i, &round, val, semaphoreList, donesem,num_proc,count));
				}
		
				for(i=0; i<num_proc; i++)
				{
			  		wait(donesem);
				}
		
				for(i=0; i<num_proc; i++)
				{
					semdelete(semaphoreList[i]);
				}
				
				retval = gettime(&end);
				if (retval == SYSERR){
					return SYSERR;
				}
				sec = end - start;
				printf("\nElapsed Seconds: %d\n",sec);
			}
			break;
   }
   
   return SHELL_OK;
}
