/*
 * Function that defines ring processes here.
 */
#include<xinu.h>
#include<stdio.h>
#include <ctype.h>
#include<process_ring.h>

/*
 *	Function Name: my_polling_proc
 *	Parameters In: Current element, next element, current process and current round.
 *	Parameters Out: process
 *	Description: Prints the Ring Element, Round and Value using polling mechanism.
 */
process my_polling_proc(int *intArray1, int *intArray2, int *pCurr, int *rCurr)
{
    int currElement = *intArray1;
    int nextElement = *intArray2;
    int rCnt = *rCurr;
    int pCnt = *pCurr;
    nextElement = currElement - 1;
    *intArray2 = nextElement;
    printf("Ring Element %d : Round %d : Value : %d\n", rCnt-1, pCnt-1, currElement); 
    return OK;
}



/*
 *	Function Name: semaphore_method
 *	Parameters In: Process Number, round number, value array, semaphore array, semaphore ID, Total number of processes and total count.
 *	Parameters Out: process
 *	Description: Prints the Ring Element, Round and Value using semaphore mechanism.
 */
process my_semaphore_proc(int32 a, int32* round, int temp[], sid32 semaphoreList[], sid32 donesem,int num_process,int count)
{
	while (*round < count)
	{
		wait(semaphoreList[a]);
		if(*round < count)
		{        
			printf("Ring Element %d : Round %d : Value : %d\n", a, *round, temp[a]);
			temp[(a+1)%num_process] = temp[a]-1;
			if(a == num_process-1)
			{
				(*round)++;
			}
		}
		
		signal(semaphoreList[(a+1) % num_process]);
    	}
	
	signal(donesem);
	return OK;
}       

