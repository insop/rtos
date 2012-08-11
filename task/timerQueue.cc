#include	"timerQueue.h"

//char *strTaskState[] = {"Dummy", "Active", "Ready", "Dead", "SendBlocked", 
	    //"ReplyBlocked", "ReceiveBlocked", "EventBlocked"};



//
//	PRIORITY QUEUE class
//
    
TimerQueue::TimerQueue() {

    numItem = 0;
    taskQueue.pHead = taskQueue.pTail = pTaskTraverse = (Task *)Empty;
    //cprintf("TimerQueue initialized \n");
    //printf("TimerQueue initialized \n");
}


void	TimerQueue::printQueue( void ) {
    Task * pTask = taskQueue.pHead;
    while(pTask != (Task *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}
	
	


/* 
void TimerQueue::printTimerQueue() {
    
    for(int i = 0; i < PriorityRange; i++) {
	//cprintf("Priority[%d] = Head: %d Tail: %d\n", i, 
	printf("Priority[%d] = Head: %d Tail: %d\n", i, 
		taskQueue.pHead, taskQueue.pTail);
    } // for
}
*/



