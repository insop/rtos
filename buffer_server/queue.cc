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
    head = tail= (DataCommnicate *)Empty;
    numItem = 0;

}

void SimpleQueue::printQueue( void ) {
// print queue
    DataCommnicate * pTask = head;
    while(pTask != (DataCommnicate *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}
