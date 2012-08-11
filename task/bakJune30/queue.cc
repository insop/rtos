//
//	simple queue queue 
//
//
// 	Written by INSOP SONG
// 	Systems Design
// 	University of Waterloo, Canada
//
//	June 2001
//

#include	"queue.h"


SimpleQueue::SimpleQueue() {
    head = tail= (Task *)Empty;
    numItem = 0;

}

void SimpleQueue::printQueue( void ) {
// print queue
    Task * pTask = head;
    while(pTask != (Task *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}
