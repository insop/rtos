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

//#define		DEBUG
//#include "../util/error.h"

#define SUN
#ifdef SUN
#include	<stdio.h>
#endif //SUN

#include "task.h"
#include "../include/util/type.h"
#include "../include/util/errorCode.h"

extern UBYTE MapTbl[];
extern UBYTE UnMapTbl[256];

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
	inline Task * 	 removeQueue(int _priority);
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
	    cprintf("Warning!!! getNextTask Error : no taks\n");
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
#endif // _PRI_QUEUE_H_

