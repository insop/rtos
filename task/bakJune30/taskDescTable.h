//
//	Task Descriptor Table
//	manage all the tasks
//
// 	Written by INSOP SONG
// 	Systems Design
// 	University of Waterloo, Canada
//
//	June 2001
//

#ifndef	_TASK_DESC_TABLE_H_
#define _TASK_DESC_TABLE_H_



#include "task.h"
#include "priQueue.h"
#include "../task/dbLinkQueue.h"
#include "../include/util/type.h"
#include "../include/util/errorCode.h"

// XXX
#define SUN
#ifdef SUN
#include <stdio.h>
#endif //SUN


class TaskDescTable 
{
    private:
	Task		aTask[MaxTaskNum];

	static int	nextTask;
	static Task*	freeTaskList;

    public:
	TaskDescTable();

// priority queue for task management
	PriQueue 	readyQueue;
// doubly linked queue for timer event handling
	DbLinkQueue	timerQueue;

	// XXX give the pTask for next user task
	inline Task *	getNextFreeTask( void );
	inline int	getMaxNumTasks( void );

	inline Task *	getPtTask( int tid );

};

// 
// 	return pTask of free task
//
inline Task * TaskDescTable::getNextFreeTask( void ) {
    Task * pTempTask;


    //if( freeTaskList->pNext == (Task *)Empty ) {
    //if( freeTaskList == (Task *)Empty ) {
    if( nextTask >= ( getMaxNumTasks() )  ) {
	printf("Warning!!!  No more free task avaiable pNext:%d\n", 
		freeTaskList->pNext);
	//printf("Warning!!!  No more free task avaiable pNext:%d\n", 
		//freeTaskList->pNext);
	getchar();
	return (Task *)Empty;
    } // if
    else {
	if( freeTaskList->pNext == (Task *) Empty) {
	    pTempTask = freeTaskList;
	} // if
	else {
	    pTempTask = freeTaskList;
	    freeTaskList = freeTaskList->pNext;
	} // else 
	    nextTask++;
	return pTempTask;
    } // else
}

//
// return task pointer from tid
//
inline Task * TaskDescTable::getPtTask( int tid ) {
    return &aTask[tid];
}

inline int TaskDescTable::getMaxNumTasks( void ) {
    return MaxTaskNum;
}


#endif //_TASK_DESC_TABLE_H_
