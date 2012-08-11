//
//	simple queue queue 
//
//
// 	Written by INSOP SONG
// 	Systems Design
//
//	Michael Cole
//	Computer science
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

#include "../include/util/type.h"
#include "../include/util/errorCode.h"

#ifndef	_SIMPLE_QUEUE_H_
#define	_SIMPLE_QUEUE_H_

typedef enum {
    Initial, Requeset_CheckIn, Requeset_CheckOut, Report_Normal,
    Report_Emergency, Finished
} msgType;

typedef struct {
    msgType 		type;
    int	 		from;
    int  		to;
    int	 		position;
    int	 		time;
    int	 		extra1;
    int  		extra2;
    struct DataCommnicate * 	pNext;
} DataCommnicate;


const BYTE       Empty                           = -1;


// 
//	Doubly linked queue 
//	mainly for event handling(timer event)
//
class	SimpleQueue {

    private:
	// queue head and tail 
	DataCommnicate *		head;
	DataCommnicate *		tail;
	int		numItem;

    public:
    // queue related member functions
	SimpleQueue();
	inline ErrorCode addQueue( DataCommnicate * _pTask );
	inline DataCommnicate *    getItem( void );
	inline int	 getNumItem( void );
	inline Boolean   isEmpty();
	void printQueue( void );	// print queue
};


inline int	 SimpleQueue::getNumItem( void ) {
    return numItem;
}
inline ErrorCode SimpleQueue::addQueue( DataCommnicate * _pTask ) {

    if( (head) == (DataCommnicate *)Empty) {
	//printf("addMsgQueue %d start tid%d\n", getTid(), _pTask->getTid());
	head = _pTask;
	tail = _pTask;
	_pTask->pNext = (DataCommnicate *)Empty;
    } // if
    else {
	//printf("addMsgQueue %d added tid%d\n", getTid(), _pTask->getTid());
	tail->pNext = _pTask;
	tail = _pTask;
	_pTask->pNext = (DataCommnicate *)Empty;
    } // else 
    numItem++;
    return True;

}

inline DataCommnicate *    SimpleQueue::getItem( void ) {

    register DataCommnicate * pTask;
    //Task * pTask;

    pTask = head;
    if( pTask->pNext != (DataCommnicate *)Empty) {
	this->head = pTask->pNext;
    } // if
    else {	
	//printf("msgQueue %d finished\n", this->getTid());
	head = (DataCommnicate *)Empty;
	tail = (DataCommnicate *)Empty;
    } // else
    numItem--;
    return pTask;


}

inline Boolean   SimpleQueue::isEmpty() {

    return (numItem == 0 ? True: False);

}

#endif // _SIMPLE_QUEUE_H_

