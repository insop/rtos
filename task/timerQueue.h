//
//	timer event queue 
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

#ifndef	_TIMER_QUEUE_H_
#define	_TIMER_QUEUE_H_


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
class	TimerQueue {

    private:
	int	numItem;

	Task *	pTaskTraverse;

    public:
// struct for head and tail pointer
	TaskQueue	taskQueue;
	TimerQueue();

	inline ErrorCode addQueue(Task * _pTask);
	inline Task *	 getNextTask();
	inline Task *	 checkNext();
	inline Task *	 removeQueue( Task * _pTask );
	inline Boolean   isEmpty();
	inline Boolean   isEnd();

	void printQueue( void );	

	//void printTimerQueue();	// print priority queue
};


inline Boolean TimerQueue::isEmpty() {
    return (numItem == 0 ? True: False);
}


//
//	return pTask
//
inline Task *	TimerQueue::getNextTask() {

    register int priority;
    register Task * pTask;
    //int priority;
    //Task * pTask;

    if( numItem > 0) {
	if(priority < 0) { // error condition
	    //Error(ErrorDebug,"getNextTask Error: no task in the queue");
	    //cprintf("getNextTask Error : no taks\n");
	    printf("getNextTask Error : no taks\n");
	    return (Task *) Empty;
	} // if
	    
	//pTask = removeQueue(priority);

	//cprintf("getNextTask tid[%d]\n", pTask->getTid());
	printf("getNextTask tid[%d] pri %d\n", 
		pTask->getTid(), pTask->getPriority());

	return pTask;
    } // if
    else 
	return (Task *) Empty;
}


inline ErrorCode TimerQueue::addQueue( Task * _pTask ) {

    if((taskQueue.pHead) == (Task *)Empty) {

	//printf("priorityQueue %d start\n", _priority);
	taskQueue.pHead = _pTask;
	taskQueue.pTail = _pTask;
	// first item, so next and previous points Empty
	_pTask->pNext = (Task *)Empty;
	_pTask->pPrev = (Task *)Empty;
    } // if
    else {
	_pTask->pPrev = taskQueue.pTail;
	taskQueue.pTail->pNext = _pTask;
	taskQueue.pTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // else 
    numItem++;
    return True;
}


// 
// 	de queue for eacy priority specific queue
//
//	return pTask;
//
//inline Task *	TimerQueue::removeQueue(int _priority) {
inline Task *	TimerQueue::removeQueue( Task * _pTask) {

    register Task * pTask;
    //Task * pTask;

    pTask = taskQueue.pHead;
    if( pTask->pNext != (Task *)Empty) {
	taskQueue.pHead = pTask->pNext;
    } // if
    else {	
	//printf("priorityQueue %d finished\n", _priority);
	taskQueue.pHead = (Task *)Empty;
	taskQueue.pTail = (Task *)Empty;
    } // else
    numItem--;
    return pTask;
}

//
//  return next task pointer
//  move the traverse pointer (pTaskTraverse)
//
inline Task *	 checkNext() {
    Task * pTask;
    return pTask;
}

// 
// check that whether it traverse to the end(tail pointer)
//
inline Boolean   isEnd() {
    return True;
}
#endif // _TIMER_QUEUE_H_

