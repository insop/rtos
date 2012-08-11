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

#include "taskDescTable.h"

int	TaskDescTable::nextTask = 0;
Task *	TaskDescTable::freeTaskList = (Task *) Empty;

TaskDescTable::TaskDescTable() {

    register int i=0;

    // 
    // link all tasks
    //

    // initially freeTaskList points the first one
    freeTaskList = &aTask[0];

    for( i = 1; i < getMaxNumTasks() ; i++ ) {
	aTask[ i - 1 ].pNext = &aTask[i];
    } // for
    
    aTask[getMaxNumTasks() - 2].pNext = &aTask[getMaxNumTasks()-1];
    aTask[getMaxNumTasks() - 1].pNext = (Task *) Empty;

    //for(i =0; i<getMaxNumTasks(); i++)
	//printf("i:%d pNext : 0x%x\n", i, (int)aTask[i].pNext);

    // test routine
    //printf("freeTaskList pointing tid%d \n", freeTaskList->tid);
    //freeTaskList = freeTaskList->pNext;
    //printf("freeTaskList+ pointing tid%d \n", freeTaskList->tid);
    //printf("freeTaskList+ pointing tid%d \n", freeTaskList->pNext->tid);
    //printf("freeTaskList253 pointing tid%d \n", aTask[253].pNext->tid);
    //printf("freeTaskList254 pointing tid%d \n", aTask[254].pNext);
    //printf("freeTaskList255 pointing tid%d \n", aTask[255].pNext);
    //getchar();
    //printf("TaskDescTable initialized \n");
}

