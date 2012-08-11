//
//	doubly linked queue queue 
//
//
// 	Written by INSOP SONG
// 	Systems Design
// 	University of Waterloo, Canada
//
//	June 2001
//

#include	"dbLinkQueue.h"

//char *strTaskState[] = {"Dummy", "Active", "Ready", "Dead", "SendBlocked", 
	    //"ReplyBlocked", "ReceiveBlocked", "EventBlocked"};



//
//	PRIORITY QUEUE class
//
    
DbLinkQueue::DbLinkQueue() {

    numItem = 0;
    numTrav = 0;
    taskQueue.pHead = taskQueue.pTail = pTaskTraverse = (Task *)Empty;
    //cprintf("DbLinkQueue initialized \n");
    //printf("DbLinkQueue initialized \n");
}


void	DbLinkQueue::printQueue( void ) {
    Task * pTask = taskQueue.pHead;
    while(pTask != (Task *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}
