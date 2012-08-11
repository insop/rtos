//
//
//
//	FEMOS(Fast EMbedded OS)
//	Real Teim Operating System project 
//
//--
//	Task 
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
//--
//

#ifndef	_TASK_H_
#define	_TASK_H_

// one level msg queue
// I don't actually use plain level msg queue
// but... jus in case 
#define PLAIN_QUEUE

// for priority inheritance
#define PRIORITY_INHERITANCE

//#define		DEBUG
//#include "../util/error.h"

#undef SUN
#ifdef SUN // if SUN
#include	<stdio.h>
#else // else SUN
#include "../include/i486/cs452.h"
#endif //SUN


// timer value decrement to zero and check
#undef	TIMER_DECREMENT_WAY

// timer value not decrement and compare to uptime 
#define	TIMER_INCREMENT_WAY

#include "../include/util/type.h"
#include "../include/util/errorCode.h"
// XXX 


#define	BITS	8


// for check number of array elements
#define	NELEMS(array)	(sizeof(array) / sizeof(array[0]))

// double dbuf[100];
// for(i = 0; i < NELEMS(dbuf); i++)
// ...


//	Task State define
enum TaskState	{ Dummy, Active, Ready, Dead, SendBlocked, ReplyBlocked, 
		    ReceiveBlocked, EventBlocked, PriorityInherited };

// XXX silly mistake but, I will fix this later
typedef TaskState TaskStae;

const int	MaxTaskNum 			= 64;
//const int	MaxTaskNum 			= 128;
//const int	MaxTaskNum 			= 256;

const int	PriorityRange			= 64;
//const int	PriorityRange			= 256;
const int	LowestPriority 			= PriorityRange - 1;

const int	HighestPriority 		= 0;

// I don't use explicit clock server and serial server
const int	ClockServerPriority 		= HighestPriority; 

// I am using name server
const int	NameServerPriority 		= HighestPriority + 1; 

// I don't use explicit clock server and serial server
const int	TrainSerialServerPriority 	= HighestPriority + 2; 
const int	WyseSerialServerPriority 	= HighestPriority + 3; 

// I don't have keyboard server yet
const int	KeybordServerPriority 		= HighestPriority + 4; 
// I don't have graphic server neither
const int	GraphicServerPriority 		= HighestPriority + 5; 

const int	SystemPriority			= GraphicServerPriority;

const int	UserPriority 			= SystemPriority + 1;

const int	InitTaskPriority 		= UserPriority;

// Idle task should be lowest priority
const int	IdleTaskPriority 		= LowestPriority;

const BYTE	Empty				= -1;


//
// --
//
// 	Task Descriptor Block class
//
// --
//

class Task
{
    private:
	// XXX assign unique Tid for each TCB Objects
	static int	nextTid;

	int		tid;
	TaskState	state;
// task priority
	int		priority;

// for priority inheritance
#ifdef	PRIORITY_INHERITANCE

	// for priority inheritance
	int		priorityOriginal;

	// XXX maybe this is not necessary.. if not, delete this later.
	int		priorityInherited;

	Boolean		isPriorityInheritanceFlag;

#endif // PRIORITY_INHERITANCE

#ifdef PLAIN_QUEUE
// msg queu head and tail for IPC managing
	Task *		msgQueueHead;
	Task *		msgQueueTail;
	int		numMsgItem;
#endif //PLAIN_QUEUE
// timer for clock event
	int		timer;

// XXX I think I need TDT wrapper class which has
// all Tasks ???
// and then it initialize and keep track all task
// and return all free tasks
// or maybe not??
// because freeTaskList or free Task ID will be tracked by
// stack member attributes.
// I have to consider about this
// XXX

// XXX give the free pTask and keep track of the Task List
//static Task*	freeTaskList;

    public:
	int		parentTid;
// pointer for queue pNext for next, pPrev for previous Task
	Task *		pNext;		
	Task *		pPrev;		

// maybe set up one register class 
// should be converted into context struct in bsp.h

	// context 	aContext
	//int *		pSP;
	//int *		pDS;
	//int *		pCS;
	//int *		pSS;
	int 		pData;		// pointer to data address

	int 		userCS;
	int 		userDS;
	int 		userSP;
	int 		userSS;

	int		requestedSysCall;
// maybe set up one class for bindfile class
	char *		taskName;	/* The name of the process */
	int 		codeSize;	/* Self explanitory variable! */
	int 		entryPoint;	/* Start executing here */
	int 		dataTotalSize;	/* Real memory pointer for data */
	int 		stackSize;	/* Size of the stack - no data */

// for message copy and handling, IPC stuffs
	Task *		pTaskSend;
	Task *		pTaskRecv;

	int		pMsgSend;
	int		msgSendLen;
	int		pMsgRecv;
	int		msgRecvLen;
//
// member functions
//
	Task();
	~Task();

	void initTask();
	//void getNext();
	ErrorCode isValidPriority(int _priority);

	void printTask();
	void hello();

	int	getTid();
	void	setParentTid(int _parentTid);
	int	getParentTid();
	// get task state in string form
	void	setState(TaskState _state);
	char *	getState();
	int 	getStateNum();

	ErrorCode setPriority(int _priority);
	int 	  getPriority();
	// XXX give the pTask for next user task
	//Task * getNextFreeTask( void );

#ifdef PLAIN_QUEUE
// msg queue related member functions
	inline ErrorCode addMsgQueue( Task * _pTask );
	inline Task *    getMsgQueue( void );
	inline Boolean   isMsgQueueEmpty();
	void printMsgQueue( void );	// print msg queue
#endif// PLAIN_QUEUE

// timer related member functions
	inline int 	  getTimer( void );
	inline void 	  setTimer( int timer );

#ifdef	TIMER_INCREMENT_WAY
	inline Boolean	  isTimeOut( int upTime );
#endif  // TIMER_INCREMENT_WAY

#ifdef  TIMER_DECREMENT_WAY
	inline void  	  decreaseTimer( void );
	inline Boolean	  isTimeOut( void );
#endif // TIMER_DECREMENT_WAY

#ifdef	PRIORITY_INHERITANCE
	inline Boolean	isPriorityInherited( void );
	inline void	setPriorityInheritance( void );		
	inline int	getOriginalPriority( void );		
	inline void	resetPriorityInheritance( void );		

	inline void 	changeInheritePriority( int _inheritedPriority);
	inline void	revertInheritePriority( void );
#endif // PRIORITY_INHERITANCE

};

inline int  Task::getTid( void ) {
    return tid;
}

inline void  Task::setParentTid(int _parentTid) {
    parentTid = _parentTid;
}

inline int  Task::getParentTid( void ) {
    return parentTid;
}

inline void  Task::setState(TaskState _state) {
    state = _state;
}

extern char *strTaskState[];

inline char *Task::getState() {
    //cprintf("task state [%s] \n", strTaskState[state]);
    //printf("task state [%s] \n", strTaskState[state]);
    return strTaskState[state];
}

inline int Task::getStateNum() {
    return state;
}
inline ErrorCode Task::setPriority(int _priority) {
    if (( priority >= LowestPriority) || (priority <= HighestPriority)) {
	priority = _priority;
	return OK;
    } // if
    else {
	return ErrorSetPriorityRange;
    }
}

#ifdef PLAIN_QUEUE

inline ErrorCode Task::addMsgQueue( Task * _pTask ) {

    if( _pTask == this ) {
	//cprintf("Warning, cannot point pNext to itself \n");
	printf("Warning, cannot point pNext to itself \n");
	getchar();
    }
	
    if( (msgQueueHead) == (Task *)Empty) {
	//printf("addMsgQueue %d start tid%d\n", getTid(), _pTask->getTid());
	msgQueueHead = _pTask;
	msgQueueTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // if
    else {
	//printf("addMsgQueue %d added tid%d\n", getTid(), _pTask->getTid());
	msgQueueTail->pNext = _pTask;
	msgQueueTail = _pTask;
	_pTask->pNext = (Task *)Empty;
    } // else 
    numMsgItem++;
    return True;
}

//
// get msg queue item 
// remove msg queue item and return pTask(point to Task)
// 
// !!!Caution : should check isEmpty() function to check 
// the size of the queue and get(=remove) the item
//
inline Task *	Task::getMsgQueue( void ) {

    register Task * pTask;
    //Task * pTask;

    pTask = this->msgQueueHead;
    if( pTask->pNext != (Task *)Empty) {
	this->msgQueueHead = pTask->pNext;
    } // if
    else {	
	//printf("msgQueue %d finished\n", this->getTid());
	this->msgQueueHead = (Task *)Empty;
	this->msgQueueTail = (Task *)Empty;
    } // else
    numMsgItem--;
    return pTask;
}


inline Boolean Task::isMsgQueueEmpty() {
    return (numMsgItem == 0 ? True: False);
}

#endif // PLAIN_QUEUE

inline int Task::getPriority() {
    //cprintf("task state [%s] \n", strTaskState[state]);
    //printf("task state [%s] \n", strTaskState[state]);
    return priority;
}

// get timer for clock event
inline int   Task::getTimer( void ) {
    return timer;
}

// set timer for clock event
inline void  Task::setTimer( int _timer ) {
    timer = _timer;
}

#ifdef TIMER_INCREMENT_WAY
inline Boolean Task::isTimeOut( int upTime ) {
    // less than zero
    return ( timer <= upTime ? True : False );
}
#endif // TIMER_INCREMENT_WAY

#ifdef TIMER_DECREMENT_WAY
// check timer is time out
inline Boolean Task::isTimeOut( void ) {
    // less than zero
    return ( timer <= 0 ? True : False );
}

inline void  Task::decreaseTimer( void ) {
    if( timer > 0) {
	timer--;
    }
}
#endif // TIMER_DECREMENT_WAY


// member functions related with priority inheritance

#ifdef	PRIORITY_INHERITANCE

inline Boolean	Task::isPriorityInherited( void ) {
    return isPriorityInheritanceFlag;
}

inline void	Task::setPriorityInheritance( void ) {
    isPriorityInheritanceFlag = True;
}

inline int	Task::getOriginalPriority( void ) {
    return priorityOriginal;
}

inline void	Task::resetPriorityInheritance( void ) {
    isPriorityInheritanceFlag = False;
}

inline void	Task::changeInheritePriority( int _inheritedPriority) {

    priorityOriginal = priority;
    priority = _inheritedPriority;
    isPriorityInheritanceFlag = True;
}

inline void	Task::revertInheritePriority( void ) {

    priority = priorityOriginal;
    isPriorityInheritanceFlag = False;
}
    

#endif // PRIORITY_INHERITANCE

#endif // _TASK_H_

