//
//
//	simple queue queue 
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

#ifndef	_SIMPLE_QUEUE_H_
#define	_SIMPLE_QUEUE_H_


// 
//	Doubly linked queue 
//	mainly for event handling(timer event)
//
class	SimpleQueue {

    private:
	// queue head and tail 
	Task *		head;
	Task *		tail;
	int		numItem;

    public:
    // queue related member functions
	SimpleQueue();
	inline ErrorCode addQueue( Task * _pTask );
	inline Task *    getItem( void );
	inline int	 getNumItem( void );
	inline Boolean   isEmpty();
	void printQueue( void );	// print queue
};


inline int	 SimpleQueue::getNumItem( void ) {
    return numItem;
}
inline ErrorCode SimpleQueue::addQueue( Task * _pTask ) {

    if( (head) == (Task *)Empty) {
	//printf("addMsgQueue %d start tid%d\n", getTid(), _pTask->getTid());
	head = _pTask;
	tail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // if
    else {
	//printf("addMsgQueue %d added tid%d\n", getTid(), _pTask->getTid());
	tail->pNext = _pTask;
	tail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // else 
    numItem++;
    return True;

}

inline Task *    SimpleQueue::getItem( void ) {

    register Task * pTask;
    //Task * pTask;

    pTask = head;
    if( pTask->pNext != (Task *)Empty) {
	this->head = pTask->pNext;
    } // if
    else {	
	//printf("msgQueue %d finished\n", this->getTid());
	head = (Task *)Empty;
	tail = (Task *)Empty;
    } // else
    numItem--;
    return pTask;


}

inline Boolean   SimpleQueue::isEmpty() {

    return (numItem == 0 ? True: False);

}

#endif // _SIMPLE_QUEUE_H_

