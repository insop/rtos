/********************************************************************
 *
 * name : sysCall.c
 * 
 *	system call, sysCall
 *
 * authors : Insop Song(System Design Engineering) 
 * 	     Michael Cole(Computer Science)
 *		
 *		University of Waterloo, ON, Canada
 *
 * functions : system call functionsfor 
 *		real time os micro kernel 
 *		cs452/cs652 class 
 *		University of Waterloo
 *
 *	sysCall.c file includes following files 
 *	:
 *	int Create( char* ptrFile,int priority )
 *	void Pass( void )
 *	void Exit( void )
 *	int MyTid( void )
 *	int MyPriority( void )
 *	int Send( int tidSend, char *ptrMsg, int msgLen,*msg,len)
 *	int Receive( char *ptrMsg, int msgLen)
 *	int ReceiveSpecific( int pid, char *ptrMsg, int msgLen)
 *	int Reply( int pid, char *ptrMsg, int msgLen)
 *	int ValidPid( int pid)
 *	int AwaitEvent( int event)
 *	int WhoIs(char * name) 
 *	int Delay(int duration) 
 *	int WakeUp(int tid);
 *	void RegisterAs(char * name) 
 *	int lengthOf(char *str)
 *  	char Get(int port)
 *  	void Put(char byte, int port)
 *  	void Write(char * msg, int len, int port)
 *  	int UpTime(void)
 *  	void DelayUntil(int time)
 * 
 * 	date : June, 2001
 *
 ********************************************************************
 */
#define  NAMESERVERPID 3

#include "../syscall/sysCall.h"
#include "../bsp/bsp.h"
/*#include "../include/util/type.h"*/
#include "../include/i486/cs452.h"
#include "../include/i486/machine/pc/timerreg.h"

/********************************************************************
 *
 * function : Create( char* ptrFile,int priority )
 * 
 * 
 * return : (int) created process id
 *
 *******************************************************************
 */

int Create( char* ptrFile,int priority )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(priority);
     PUSH_VAR(ptrFile);

     PUSH_RET_VAL;

     PUSH_VAR(CREATE_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);

     POP(2);		 /* pop the number of inputted parameter */
     CHECK_MAGIC;

     return returnValue;
}


/********************************************************************
 *
 * function : Pass()
 * 
 * 
 * return : NO 
 *
 *******************************************************************
 */
void Pass( void )
{
     PUSH_MAGIC;
     PUSH_VAR(PASS_SYS_CALL);
     INTERRUPT_SOFT;
     CHECK_MAGIC;
}


/********************************************************************
 *
 * function : Exit()
 * 
 * 
 * return : NO 
 *
 *******************************************************************
 */
void Exit( void )
{
     PUSH_MAGIC;
     PUSH_VAR(EXIT_SYS_CALL);
     INTERRUPT_SOFT;
     CHECK_MAGIC;
}

/********************************************************************
 *
 * function : MyTid()
 * 
 * 
 * return : (int) process id 
 *
 *******************************************************************
 */
int MyTid( void )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_RET_VAL;
     PUSH_VAR(MYPID_SYS_CALL);
     INTERRUPT_SOFT;
     POP_RET_VAL(returnValue);
     CHECK_MAGIC;
     return returnValue;
}

/********************************************************************
 *
 * function : MyParentTid()
 * 
 * 
 * return : (int) parent process id 
 *
 *******************************************************************
 */
int MyParentTid( void )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_RET_VAL;

     PUSH_VAR(PARENTPID_SYS_CALL)
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     CHECK_MAGIC;
     return returnValue;
}
/********************************************************************
 *
 * function : MyPriority()
 * 
 * 
 * return : (int) process priority 
 *
 *******************************************************************
 */
int MyPriority( void )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_RET_VAL;

     PUSH_VAR(MYPRIORITY_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     CHECK_MAGIC;
     return returnValue;
}

/********************************************************************
 *
 * function : Send( int tidSend, void *ptrIn, int inLen, 
 *			void *ptrOut, int outLen)
 * 
 * 
 * return : (int)  value of the real size of received buffer message 
 *
 *******************************************************************
 */
int Send( int tidSend, void *ptrIn, 
	    int inLen, void *ptrOut, int outLen)
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(outLen);
     PUSH_VAR(ptrOut);
     PUSH_VAR(inLen);
     PUSH_VAR(ptrIn);
     PUSH_VAR(tidSend);

     PUSH_RET_VAL;

     PUSH_VAR(SEND_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);

     POP(5); /* pop the number of inputted parameter */
     CHECK_MAGIC;

     return returnValue;
}

/********************************************************************
 *
 * function : Receive( void *ptrMsg, int msgLen)
 * 
 *
 * !!! you can assign the sender pid or if you do not have 
 * particular sender you just put UNIV_SEND(defined in sysCall.h)
 * 
 * return : (int)  value of the PID of sender 
 *
 *******************************************************************
 */
int Receive( void *ptrMsg, int msgLen)
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(msgLen);
     PUSH_VAR(ptrMsg);

     PUSH_RET_VAL;

     PUSH_VAR(RECEIVE_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);

     POP(2); /* pop the number of inputted parameter */
     CHECK_MAGIC;

     return returnValue;
}

/********************************************************************
 *
 * function : ReceiveSpecific( int hearFome, void *ptrMsg, int msgLen)
 * 
 *
 * return : (int)  value of the PID of sender 
 *
 *******************************************************************
 */
int ReceiveSpecific( int hearFrom, int *ptrMsg,  int msgLen)
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(msgLen);
     PUSH_VAR(hearFrom);
     PUSH_VAR(ptrMsg);

     PUSH_RET_VAL;

     PUSH_VAR(RECEIVE_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);

     POP(3); /* pop the number of inputted parameter */
     CHECK_MAGIC;

     return returnValue;
}
/********************************************************************
 *
 * function : Reply( int pid, char *ptrMsg, int msgLen)
 * 
 * 
 * return : (int)  
 *
 *******************************************************************
 */
int Reply( int pid, void *ptrMsg, int msgLen)
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(msgLen);
     PUSH_VAR(ptrMsg);
     PUSH_VAR(pid);

     PUSH_RET_VAL;

     PUSH_VAR(REPLY_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);

     POP(3); /* pop the number of inputted parameter */
     CHECK_MAGIC;

     return returnValue;
}

char Get( int port )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(port);

     PUSH_RET_VAL;

     PUSH_VAR(GET_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     POP(1);
     CHECK_MAGIC;

     return returnValue;
}

char Put( char _data, int port )
{
     int	returnValue;
     int data = _data;
     PUSH_MAGIC;
     PUSH_VAR(port);
     PUSH_VAR(data);

     PUSH_RET_VAL;

     PUSH_VAR(PUT_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     POP(2);
     CHECK_MAGIC;

     return returnValue;
}

/* 
//
//  Read : Blocking serial read
// 	returns number of inputted data
// 	if some other call came first 
//	return EMPTY(-1)
//
//
//  this call should be called by only one task
//	this is not good but for the time being....
//	if there is one task who called this first
//	this just return EMPTY(-1)
//
*/
int Read(void * _data, int port) {

    int		returnValue;
    PUSH_MAGIC;
    PUSH_VAR(port);
    PUSH_VAR(_data);

    PUSH_RET_VAL;

    PUSH_VAR(READ_SYS_CALL);
    INTERRUPT_SOFT;

    POP_RET_VAL(returnValue);
    POP(2);
    CHECK_MAGIC;
     
    return returnValue;
}

char Write( void * _data, int len, int port )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(port);
     PUSH_VAR(len);
     PUSH_VAR(_data);

     PUSH_RET_VAL;

     PUSH_VAR(WRITE_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     POP(3);
     CHECK_MAGIC;

     return returnValue;
}
/********************************************************************
 *
 * function : ValidPid( int pid )
 * 
 * 
 * return : (int) non zero or zero  
 *
 *******************************************************************
 */
int ValidPid( int pid)
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(pid);

     PUSH_RET_VAL;

     PUSH_VAR(VALIDPID_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     POP(1);
     CHECK_MAGIC;

     return returnValue;
}

/********************************************************************
 *
 * function : AwaitEvent( int event )
 * 
 * 
 * return : (int) result 
 *
 *******************************************************************
int AwaitEvent( int event)
 */
int AwaitEvent( int eventType )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR( eventType );

     PUSH_RET_VAL;

     PUSH_VAR( AWAITEVENT_SYS_CALL );
     INTERRUPT_SOFT;

     POP( 1 );
     POP_RET_VAL( returnValue );
     CHECK_MAGIC;
     return returnValue;
}


/********************************************************************
 *
 * function : UpTime()
 * 
 * 
 * return : (int) kernel operation time 
 *
 *******************************************************************
 */
int UpTime( void )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_RET_VAL;
     PUSH_VAR(UPTIME_SYS_CALL);
     INTERRUPT_SOFT;
     POP_RET_VAL(returnValue);
     CHECK_MAGIC;
     return returnValue;
}

/*
// 
//  read counter value directly from 8254
//  8254 is decrement counter 
//  this should be used with UpTime() to 
//  check the overflow(sort of )
//
*/
int GetSmallTime( void ) {
    /*
    // Read the output pin of Counter1 of 8053
    */
    register int currentTimerCount;
    outb(TIMER_MODE, TIMER_SEL1|TIMER_LATCH);
    currentTimerCount = inb(TIMER_CNTR1) & 0xff;
    currentTimerCount |= (inb(TIMER_CNTR1)<<8); 

    return currentTimerCount;
}


/*
    counterDiff = oldTimerCount - currentTimerCount;
    oldTimerCount = currentTimerCount;
*/


/********************************************************************
 *
 * function : void Delay(int)
 *
 * when woken up by WakeUp return value is (-1)
 * otherwise don't care about return value
 *
 *******************************************************************
 *xdelay
 */

int Delay(int duration) {

     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(duration);

     PUSH_RET_VAL;

     PUSH_VAR(DELAY_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL( returnValue );
     POP( 1 );
     CHECK_MAGIC;
     return returnValue;
}

/********************************************************************
 *
 * function : void WakeUp(int tid)
 *
 *******************************************************************
 *xdelay
 */

int WakeUp(int tid) {

     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(tid);

     PUSH_RET_VAL;

     PUSH_VAR(WAKE_UP_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL( returnValue );
     POP( 1 );
     CHECK_MAGIC;
     return returnValue;
}


/********************************************************************
 *
 * function : RegisterAs( char* ptrName )
 * 
 * 
 * return : 
 *
 *******************************************************************
 */

int RegisterAs( char* ptrName ) {

    NameServiceMsg	msg;

    if ( strlen(ptrName) > NAME_SIZE ) {
	cprintf("RegisterAs: name is too long %s."
	" Maximum is %d", ptrName, NAME_SIZE );
	printf("RegisterAs: name is too long %s."
	" Maximum is %d", ptrName, NAME_SIZE );
	return -1;
    }

    msg.type = Register;
    strcpy(msg.name, ptrName);

/* I should check the result, I will do later on */
    Send( NAME_SERVER_TID, (void *) &msg, sizeof( msg ),
		    NULL,NULL);

    return 1;
}

/********************************************************************
 *
 * function : WhoIs( char* ptrName )
 * 
 * 
 * return : 
 *
 *******************************************************************
 */

int WhoIs( char* ptrName ) {

    NameServiceMsg	msg;
    int	tid_lookup;

    if ( strlen(ptrName) > NAME_SIZE ) {
	cprintf("WhoIs: name is too long %s."
	" Maximum is %d", ptrName, NAME_SIZE );
	printf("WhoIs: name is too long %s."
	" Maximum is %d", ptrName, NAME_SIZE );
	return -1;
    }

    msg.type = LookUp;
    strcpy(msg.name, ptrName);

/* I should check the result, I will do later on */
    Send( NAME_SERVER_TID, (void *) &msg, sizeof( msg ),
		    &(tid_lookup),sizeof(tid_lookup));

    return tid_lookup;
}

/********************************************************************
 *
 * function : void DelayUntil(int)
 * 
 *
 *******************************************************************
 *xdelay
 */

int DelayUntil(int specfiedFutureTime) {

     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(specfiedFutureTime);

     PUSH_RET_VAL;

     PUSH_VAR(DELAY_UNTIL_SYS_CALL);
     INTERRUPT_SOFT;

     POP( 1 );
     POP_RET_VAL( returnValue );
     CHECK_MAGIC;
     return returnValue;
}
/********************************************************************
 *
 * function : void AbsMem(int)
 *
 *	get absolute memeory address of the task
 * 	pTask->pData
 *
 *******************************************************************
 *xdelay
 */
int GetAbsMem( int tid )
{
     int	returnValue;
     PUSH_MAGIC;
     PUSH_VAR(tid);

     PUSH_RET_VAL;

     PUSH_VAR(GET_ABS_MEM_SYS_CALL);
     INTERRUPT_SOFT;

     POP_RET_VAL(returnValue);
     POP(1);
     CHECK_MAGIC;

     return returnValue;
}


inline unsigned strLen(const char *s) {
    register int n;

    for( n = 0; *s != '\0'; ++s)
	++n;

    return n;
}
