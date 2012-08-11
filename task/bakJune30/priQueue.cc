//
//	Priority queue 
//
//
// 	Written by INSOP SONG
// 	Systems Design
// 	University of Waterloo, Canada
//
//	June 2001
//

#include	"priQueue.h"

//char *strTaskState[] = {"Dummy", "Active", "Ready", "Dead", "SendBlocked", 
	    //"ReplyBlocked", "ReceiveBlocked", "EventBlocked"};



//
//	PRIORITY QUEUE class
//
    

PriQueue::PriQueue() {

    numItem = 0;
    int i = 0;
    for( i = 0; i < PriorityRange; i++) {
	taskQ[i].pHead = taskQ[i].pTail = (Task *)Empty;
    } // for


    rdyGrp      = 0;                                     /* Clear the ready list                     */
    for (i = 0; i < 8; i++) {
        rdyTbl[i] = 0;
    }
    //cprintf("PQ initialized \n");
    //printf("PriQueue initialized \n");
}

#undef NOT_USE
#ifdef NOT_USE
// 
// basic getHighPriority by linear search
//
int	PriQueue::getHighestPriority(void) {

    for(int priority = HighestPriority; priority <= LowestPriority ;priority++) {
	
	if( taskQ[priority].pHead != (Task *)Empty) {
	    //cprintf("Highest Priority is %d\n",priority);
	    //printf("Highest Priority is %d\n",priority);
	    return priority;
	}  // if
    }  // for 
    return ErrorGetHighPriority;
}
#endif // NOT_USE




void	PriQueue::printQueue(int p) {
    Task * pTask = taskQ[p].pHead;
    while(pTask != (Task *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}
	
	


    
void PriQueue::printPriQueue() {
    
    for(int i = 0; i < PriorityRange; i++) {
	//cprintf("Priority[%d] = Head: %d Tail: %d\n", i, 
	printf("Priority[%d] = Head: %d Tail: %d\n", i, 
		taskQ[i].pHead, taskQ[i].pTail);
    } // for
}



