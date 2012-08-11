//
//
//
//	doubly linked queue queue 
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//
//
//	Michael Cole Computer science
//
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

#ifndef	_DBLINK_QUEUE_H_
#define	_DBLINK_QUEUE_H_


// // 	Queue structure
//
typedef struct	QueueBase {

	Task *	pHead;
	Task *	pTail;
}TQueue;

// 
//	Doubly linked queue 
//	mainly for event handling(timer event)
//
class	DbLinkQueue {

    private:
	int	numItem;
	int	numTrav;

	Task *	pTaskTraverse;

    public:
// struct for head and tail pointer
	TQueue	taskQueue;
	DbLinkQueue();

	inline ErrorCode addQueue (Task * _pTask);
	inline Task *	 getNextElement ( void );
	inline Task *    getHead( void );
	inline int	 getSize(void);
	inline ErrorCode removeElement ( Task * _pTask );
	inline Boolean   isEmpty ( void );
	inline void	 resetTravPointer ( void );
	void printQueue ( void );	

//XXX not use yet
	inline Boolean   isTravEnd ( void );
	inline Task *	 checkNext ( void );
};


inline Boolean DbLinkQueue::isEmpty() {
    return (numItem == 0 ? True: False);
}

inline int DbLinkQueue::getSize( void ) {
    return numItem;
}

inline Task *	DbLinkQueue::getHead() {
    return  taskQueue.pHead;
}
//
//	!!! Caution, just return pTask NOT REMOVE from the 
// 	doublly linked list
//	return pTask
//
inline Task *	DbLinkQueue::getNextElement() {

    if( numTrav > 0) {
	if( pTaskTraverse == (Task*) Empty) {
	// when we start traverse
	    pTaskTraverse = taskQueue.pHead;
	    //printf("getNextElement first one of DBLINK tid[%d] pri %d\n", 
		//pTaskTraverse->getTid(), pTaskTraverse->getPriority());
	    numTrav--;
	    return pTaskTraverse;
	} // if
	else {
	// after first element
	    pTaskTraverse= pTaskTraverse->pNext;
	    //printf("getNextElement of DBLINK tid[%d] pri %d\n", 
		//pTaskTraverse->getTid(), pTaskTraverse->getPriority());
	    numTrav--;
	    return pTaskTraverse;
	} //else 
    } // if
    else 
	return (Task *) Empty;
}


inline ErrorCode DbLinkQueue::addQueue( Task * _pTask ) {

    if((taskQueue.pHead) == (Task *)Empty) {
	//cprintf("timer queue tid%d start\n", _pTask->getTid());
	taskQueue.pHead = _pTask;
	taskQueue.pTail = _pTask;
	// first item, so next and previous points Empty
	_pTask->pNext = (Task *)Empty;
	_pTask->pPrev = (Task *)Empty;
    } // if
    else {
	//cprintf("timer queue tid%d \n", _pTask->getTid());
	_pTask->pPrev = taskQueue.pTail;
	taskQueue.pTail->pNext = _pTask;
	taskQueue.pTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // else 
    numItem++;
    numTrav++;
    return True;
}


// 
// 	remove element from 
//	doubly linked queue
//
//	return pTask;
//
inline ErrorCode DbLinkQueue::removeElement( Task * _pTask) {

    register Task * pTask;
// dbLinkQueue should not be empty 
// and _pTask should not be empty(=NULL)
    if( (!isEmpty()) && (_pTask != (Task*)Empty) ) {
	//XXX we have to think that when the one element comes
	if( _pTask->pPrev == (Task*) Empty ) {
// first element remove
	    if( _pTask->pNext == (Task*) Empty ) {
		// only one element
		taskQueue.pTail = taskQueue.pHead = (Task *) Empty;
	    }
	    else  {
		taskQueue.pHead = _pTask->pNext;
		taskQueue.pHead->pPrev = (Task*)Empty;
	    }
	} // if
	else if( _pTask->pNext == (Task*) Empty ) {
// last one remove
	   taskQueue.pTail = _pTask->pPrev;
	   taskQueue.pTail->pNext = (Task*)Empty;
	} // else if
	else {
// middle ones remove
	    _pTask->pPrev->pNext = _pTask->pNext;
	    _pTask->pNext->pPrev = _pTask->pPrev;
	} // else 
	numItem--;
	return OK;
    } // if
    else 
	return ErrorNullTaskPtRemove;
}
/*
    pTask = taskQueue.pHead;
    if( pTask->pNext != (Task *)Empty) {
	taskQueue.pHead = pTask->pNext;
    } // if
    else {	
	//printf("priorityQueue %d finished\n", _priority);
	taskQueue.pHead = (Task *)Empty;
	taskQueue.pTail = (Task *)Empty;
    } // else
    return pTask;
*/

//
//  return next task pointer
//  move the traverse pointer (pTaskTraverse)
//
//XXX not use yet
inline Task * DbLinkQueue::checkNext() {
    Task * pTask;
    return pTask;
}

//
// reset traverse pointer(=pTaskTraverse)
// for next traverse the queue
//
inline void DbLinkQueue::resetTravPointer( void ) {
    pTaskTraverse = (Task*) Empty;
    numTrav = numItem;
}

// 
// check that whether it traverse to the end(tail pointer)
//
inline Boolean DbLinkQueue::isTravEnd() {
    return (numTrav == 0 ? True: False);
}

#undef SAMPLE
#ifdef SAMPLE
//
// !!! Caution !!!
// example of using traverse doubly liked queue
// XXX should use dbLinkQueue this way
//
// aTDTable   : TaskDescTable Class
// timerQueue : this doubly linked queue itself
// pTestTask  : Task pointer
// XXX whey you traver check isTravEnd() function everytime.
    while( !aTDTable.timerQueue.isTravEnd()) {
	pTestTask = aTDTable.timerQueue.getNextElement();
// if you want remove element DO like this
	aTDTable.timerQueue.removeElement(pTestTask);
	printf("timer queue traversing, get tid %d<<<\n", pTestTask->getTid());
    }
// XXX should reset the traverse pointer
    aTDTable.timerQueue.resetTravPointer();
#endif // SAMPLE
#endif // _DBLINK_QUEUE_H_

