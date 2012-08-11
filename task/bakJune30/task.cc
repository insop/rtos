//
//	Task  
//
//
// 	Written by INSOP SONG
// 	Systems Design
// 	University of Waterloo, Canada
//
//	June 2001
//

#include	"../task/task.h"


char *strTaskState[] = {"Dummy", "Active", "Ready", "Dead", "SendBlocked", 
	    "ReplyBlocked", "ReceiveBlocked", "EventBlocked"};



int	Task::nextTid = 0;

//
//	TASK CLASS
//
Task::Task() {
    state = Dummy; 

    tid = Task::nextTid++;
    priority = LowestPriority;

//  initialize message queue head and tail & and numMsgItem 
    msgQueueHead = msgQueueTail= (Task *)Empty;
    numMsgItem = 0;

    //printf("Task initialized and TID %d\n", tid);

    // XXX connect all Task class using pNext and 
    // point first one with freeTaskList and then later
    // keep track this one with getNextFreeTask() function
    // XXX 


    //cprintf("Task created tid[%d] priority[%d]\n", tid, priority);
    //printf("Task created tid[%d] priority[%d]\n", tid, priority);
}

Task::~Task() {
    //cprintf("Task Destoried \n");
    //printf("Task Destoried \n");
}

void Task::hello() {
    //cprintf("Task Hello \n");
    //printf("Task Hello \n");
}


void Task::printTask()	{
    //printf("tid[%d], pri[%d], s[%s]\n", 
    printf("tid[%d], pri[%d], s[%s]\n", 
	Task::tid, Task::priority, Task::getState());
}

void Task::printMsgQueue( void ) {
// print msg queue
    Task * pTask = msgQueueHead;
    while(pTask != (Task *)Empty) {
	pTask->printTask();
	pTask = pTask->pNext;
    }
}

ErrorCode Task::isValidPriority(int _priority) {

    if( ( _priority > PriorityRange - 1) ||
	( _priority < HighestPriority ))
	return ErrorPriority;
    else
	return OK;
}


