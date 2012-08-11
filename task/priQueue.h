//
//
//--
//
//
//	FEMOS(Fast EMbedded OS) kernel main  
//
//
//--
//
//
//	Priority queue 
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//
//	Michael Cole Computer science
//
// 	University of Waterloo, ON, Canada
//
//	June 2001
//

//#define		DEBUG
//#include "../util/error.h"

#define SUN
#ifdef SUN
#include	<stdio.h>
#endif //SUN

#include "task.h"
#include "../include/util/type.h"
#include "../include/util/errorCode.h"

#ifndef	_PRI_QUEUE_H_
#define	_PRI_QUEUE_H_

// 
// 	Queue structure
//
typedef struct	Queue {

	Task *	pHead;
	Task *	pTail;
}TaskQueue;

// 
//	Priority Queue for Task Descriptor Block
//
class	PriQueue {

    private:
	UBYTE	rdyGrp;
	UBYTE	rdyTbl[8];

	int	numItem;

    public:
	TaskQueue	taskQ[PriorityRange];	// priority queue
	PriQueue();

	//inline Boolean isEmpty(int _priority);

	inline ErrorCode addPriQueue(Task * _pTask);
	inline ErrorCode addQueue(Task * _pTask, int _priority);
	inline Task *	 getNextTask();

	inline Boolean   isEmpty();
	void printQueue(int _priority);	// print specific priority
	void printPriQueue();	// print priority queue

	//XXX 
	inline void	 setPriorityMask(int _priority);
	//XXX 
	inline void	 clearPriorityMask(int _priority);
	//XXX 
	inline int 	 getHighestPriority(void);

	inline Task * 	 removeQueue(int _priority);

	// remove node (Task) from the priority Queue
	// cf> Caution using with the removeQueue() function
	inline Boolean	removePriQueue( Task * _pTask );

};


static UBYTE const MapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

static UBYTE const UnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

inline Boolean PriQueue::isEmpty() {
    return (numItem == 0 ? True: False);
}

//
//	return pTask
//	and !!! Remove from the queue
//
inline Task *	PriQueue::getNextTask() {

    register int priority;
    register Task * pTask;
    //int priority;
    //Task * pTask;

    if( numItem > 0) {
	priority = getHighestPriority();
	if(priority < 0) { // error condition
	    //Error(ErrorDebug,"getNextTask Error: no task in the queue");
	    //cprintf("getNextTask Error : no taks\n");
	    //cprintf("Warning!!! getNextTask Error : no taks\n");
	    printf("Warning!!! getNextTask Error : no taks\n");
	    getchar();
	    return (Task *) Empty;
	} // if
	    
	pTask = removeQueue(priority);

	//cprintf("getNextTask tid[%d]\n", pTask->getTid());
	//printf("getNextTask tid[%d] pri %d\n", 
		//pTask->getTid(), pTask->getPriority());

	return pTask;
    } // if
    else 
	return (Task *) Empty;
}


inline ErrorCode PriQueue::addPriQueue(Task * _pTask) {

    register int priority = _pTask->getPriority();
    register int tid = _pTask->getTid();

    //int priority = _pTask->getPriority;
    //int tid = _pTask->getTid();

    // XXX 
    //_pTask->setState(Ready);

    //cprintf("EnQueueing tid[%d], priority[%d]\n", tid, priority);
    //printf("add priQueueing tid[%d], priority[%d]\n", tid, priority);

    return addQueue(_pTask, priority);
}

// 
// 	add queue for each priority specific queue
//

inline ErrorCode PriQueue::addQueue(Task * _pTask, int _priority) {

    if((taskQ[_priority].pHead) == (Task *)Empty) {

	setPriorityMask(_priority);	// this is the first, so set
	//printf("priorityQueue %d start\n", _priority);
	taskQ[_priority].pHead = _pTask;
	taskQ[_priority].pTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // if
    else {
	taskQ[_priority].pTail->pNext = _pTask;
	taskQ[_priority].pTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // else 
    numItem++;
    return True;
}
inline void	PriQueue::setPriorityMask(int p) {

    rdyGrp 	    |= MapTbl[ p >> 3 ];
    rdyTbl[p >>3 ]  |= MapTbl[ p & 0x07 ];

}

inline void	PriQueue::clearPriorityMask(int p) {

    if((rdyTbl[ p >> 3 ] &= ~MapTbl[ p & 0x07 ]) == 0 )
		rdyGrp	 &= ~MapTbl[ p >> 3 ];
}

// 
//	get highest priority 
//	by bit mapping
//
// 	can be changed into inline
//
inline int	PriQueue::getHighestPriority( void ) {
    register x,y,priority;
    y = UnMapTbl[ rdyGrp ];
    x = UnMapTbl[ rdyTbl[ y ] ];
    priority = ( y << 3) + x; 
    return priority;
}


// 
// 	de queue for eacy priority specific queue
//
//	return pTask;
//
inline Task *	PriQueue::removeQueue(int _priority) {

    register Task * pTask;
    //Task * pTask;

    pTask = taskQ[_priority].pHead;
    if( pTask->pNext != (Task *)Empty) {
	taskQ[_priority].pHead = pTask->pNext;
    } // if
    else {	
	clearPriorityMask(_priority);	// last task of the priority
	//printf("priorityQueue %d finished\n", _priority);
	taskQ[_priority].pHead = (Task *)Empty;
	taskQ[_priority].pTail = (Task *)Empty;
    } // else
    numItem--;
    return pTask;
}

//
//	remove the given task(pTask)
//	from the queue
// 	and return the result(Boolean)
//	cf> Task * removeQueue(int priority)
//
inline Boolean	PriQueue::removePriQueue( Task * pRmvTask ) {

    register int priority = pRmvTask->getPriority();
    register Task * head;
    register Task * tail;
    register Task * current;
    register Task * previous;
    register char loopCnt=0;
    register Boolean found = False;

    head = previous = current = taskQ[priority].pHead;
    tail = taskQ[priority].pTail;

    // if there is no task for this priority
    //	then just return 
    if( head == (Task *) Empty ) {
	cprintf("Warning try to remove invalid task \n");
	printf("Warning try to remove invalid task[%d] \n", pRmvTask->getTid());
	return False;
    }

    //printf(">>removed find priority %d\n", priority);

    // loop until it finds or to the end
    do {
	    //  if not the one that we want
	if( current != pRmvTask ) {

	    if( current != head) {
		previous = current;
		current = current->pNext;
	    } // inner else
	    else {
		previous = head;
		current = head->pNext;
	    } // inner if
	} // if
	// bingo, we find the one to remove
	else {
	    // if the reomved the node was the last one
	    if(head == tail) {
		//printf("the only one was removed \n");
		// if the removed task is the last one of the priority
		clearPriorityMask(priority);	
		//printf("priorityQueue %d finished\n", _priority);
		taskQ[priority].pHead = (Task *)Empty;
		taskQ[priority].pTail = (Task *)Empty;
		found = True;
	    }
	    else if (current == head) {
		//printf("the first one removed removed \n");
		taskQ[priority].pHead = current->pNext;
	    }
	    else {
		//printf("NOT the only one was removed \n");
		previous->pNext = current->pNext;
	    }
	    found = True;
	} // else

    } while( !(current == tail) && !(found == True)); 

    if( (current == tail) && (current == pRmvTask) ) {
	//printf("Last one was removed \n");
	taskQ[priority].pTail = previous;
	previous->pNext = (Task *) Empty;
	found = True;
    } // if

    if( found == False) {
	cprintf("Cannot found for remove. Invalid pTask \n");
	printf("Cannot found for remove. Invalid pTask \n");
	//cprintf("Cannot found for remove. Invalid pTask \n");
	return False;
    } // if
    //printf(">>remove tid %d loopCnt %d\n", current->getTid(), loopCnt);
    numItem--;
    return True;
}

#endif // _PRI_QUEUE_H_

