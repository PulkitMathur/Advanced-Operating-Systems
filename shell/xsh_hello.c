
/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - Say hello to XINU
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {	
	/* Check argument count */
	if (nargs < 2) 
	{
		/*Number of arguments less than 2*/
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	
	if (nargs > 2) 
	{
		/*Number of arguments more than 2*/
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

	if (nargs == 2) 
	{
		/*Required Number of arguments*/
		printf("Hello ");
		printf("%s",args[1]);
		printf(", Welcome to the world of Xinu!!");
		printf("\n");
	}
	
	return 0;
}
