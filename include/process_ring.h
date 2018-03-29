/* process_ring.h */
#include<xinu.h>

/*
 *	Function declaration for polling and semaphore mechanism.
 */
extern process my_polling_proc(int*, int*, int*, int*);
extern process my_semaphore_proc(int32,int32*,int [],sid32 [], sid32 ,int ,int);


