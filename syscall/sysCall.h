/********************************************************************
 *
 * name : sysCall.h
 *
 * authors : Insop Song(System Design Engineering) 
 * 	     Michael Cole(Computer Science)
 *
 * functions : header file of
 *		sysCall.c(system call functions for 
 *		real time os micro kernel)
 *		cs452/cs652 class 
 *		University of Waterloo
 *
 *	sysCall.h file includes following files 
 *	:
 *	int Create( char* ptrFile,int priority )
 *	void Pass( void )
 *	void Exit( void )
 *	int MyPid( void )
 *	int MyPriority( void )
 *	int Send( int tidSend, char *ptrMsg, int msgLen)
 *	int Receive( char *ptrMsg, int msgLen)
 *	int ReceiveSpecific( int pid, char *ptrMsg, int msgLen)
 *	int Reply( int pid, char *ptrMsg, int msgLen)
 *	int lengthOf(char *str)
 *	void Int2Asc(int, char *)
 *	int Asc2Int(char *)
 *	int ValidPid( int pid)
 *	int Delay(int duration)
 *	int WakeUp(int tid);
 *	void RegisterAs(char * name)
 *	int WhoIs( char * name)
 *	char Get(int port)
 *	void Put(char byte, int port)
 *	void Write(char * msg, int port, int len)
 *	int UpTime(void)
 *	int GetSmallTime( void ) {
 *	void DelayUntil(int time)
 *	int GetAbsMem( int tid);
 * 
 * date : 27 May, 2001
 *
 ********************************************************************
 */
#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

/*
#define MAXNAMESIZE 	50
*/
#define MAXMSGSIZE 	50
#define MAX_WRITE_DATA 	200
#define MAX_READ_DATA 	200
#define CAPOFFSET 	12

/* because namesrv is unstable */
#define KERNEL_TID	0
#define INIT_TID	1
#define IDLE_TPID	2
#define STAT_TID	3
#define NAME_SERVER_TID	4

#define MAX_CNT_MAGIC	0xabcdabcd
#define IDLE_CNT_MAGIC	0x12345678

#define LOWEST_PRI	63
#define MAX_TASK_NUM	63	/* 64 but one for kernel */


#define TICKS_PER_SEC 	20	/* interrupt timer 50 ms */
/* #define TICKS_PER_SEC 	100	*//* interrupt timer 10 ms */

#define EVENT_NUM		7	/* number of event */

#define CLOCK_EVENT		0	/* timer event */
#define SERIAL0_READ_EVENT	1	/* serial port 0 event */
#define SERIAL0_WRITE_EVENT	2	/* serial port 0 event */
#define SERIAL1_READ_EVENT	3	/* serial port 0 event */
#define SERIAL1_WRITE_EVENT	4	/* serial port 0 event */


#define WYSE	1
#define TRAIN	0

/* magic number for shared memory */
#define SHARED_MAGIC	0x13


/* name server related definictions */
#define	NAME_SIZE	20

typedef enum { Register, LookUp } NameServiceType;

typedef struct {
    NameServiceType	type;
    char		name[ NAME_SIZE + 1];
} NameServiceMsg;


typedef enum {
	INVALID_SYS_CALL,
	CREATE_SYS_CALL,
	PASS_SYS_CALL,
	EXIT_SYS_CALL,
	MYPID_SYS_CALL,
	PARENTPID_SYS_CALL,
	MYPRIORITY_SYS_CALL,
	SEND_SYS_CALL,
	RECEIVE_SYS_CALL,
	REPLY_SYS_CALL,
	VALIDPID_SYS_CALL,
	AWAITEVENT_SYS_CALL,
	DELAY_SYS_CALL,
	GET_SYS_CALL,
	PUT_SYS_CALL,
	READ_SYS_CALL,
	WRITE_SYS_CALL,
	UPTIME_SYS_CALL,
	DELAY_UNTIL_SYS_CALL,
	GET_ABS_MEM_SYS_CALL,
	WAKE_UP_SYS_CALL,
	LAST_NUM_SYS_CALL
} SysCallType;


#define PUT	1
#define GET	2


#define	MAGIC1	0x12345678
#define	MAGIC2	0xABCDABCD

/* include this UNIVS_SEND definition 
 * only if TaskDescriptorTable.h
 * is not included
 * which in case of the user program, 
 * not in the case of the kernel program
 */
#ifndef _TaskDescriptorTable_H_
#endif //_TaskDescriptorTable_H_

static int	null,popCount;
static int	magicCheck;
#define PUSH_MAGIC  	PUSH_VAR(MAGIC1);
#define CHECK_MAGIC \
	    __asm volatile("popl _magicCheck");	\
	       if( magicCheck != MAGIC1 ) {		\
		 printf("CHECK MAGIC NUMBER ERROR" \
		 " (%d)\n", magicCheck );	\
		 asm ("cli;hlt\n");		\
	       }
#define PUSH_RET_VAL	PUSH_VAR(MAGIC2);

#define PUSH_VAR(x)\
	    __asm volatile("pushl %%eax":: "a" (x) : "%eax" );
#define INTERRUPT_SOFT \
	    __asm volatile("int $69\n\t" \
				       "popl _null\n\t");
#define POP_RET_VAL(x) \
	    __asm volatile("popl %%eax": "=a" (x) :: "%eax" );
#define POP(x)	\
	    for( popCount=0; popCount<x; popCount++ ) { \
	      asm("popl _null"); \
	    }

#define enterCS() __asm__ __volatile__ ("cli");

#define exitCS() __asm__ __volatile__ ("sti");

/* Create a process. Note, in creating an instruction:
* First create a stack space and save SS in the TDT.
* Then push the CS onto the task stack (movw %cs [SS:ESI])
* Then copy data, leave enough room for BSS, push DS.
* Finally save ESI.
*/
int Create(char*, int);

/* Don't bother with resource reclimation - it takes too long */
void Exit();

/* Context switch.
* Save SS in the TDT. Then push CS onto the stack.
* Then push DS, finnally Save ESI, call interupt. (int n).
* Now dig DS off of the user stack, (get the user stack data from
* the TDT) then recover CS. Set SS and ESI to be equal to the
* value in the TDT. Run a return from interupt.
*/
void Pass();

/* Return the PID of the active process */
int MyPid();
/* Return the Priority of the active process */
int MyPriority();

/* Convert integer into a string for easy message passing */
void Int2Asc(int i, char * buff);

int Send(int tidSend,void *pMsgSrc,int srcLen,void *pMsgDes,int desLen);
int Receive(void *ptrMsg, int msgLen);
int Reply(int tid, void *ptrMsg, int msgLen);

/* Return an integer representation of data in a string */
int Asc2Int(char * buff);

/* Block a process on the clock server for duration ticks */
int Delay(int duration);

/* Register this user process with the name server */
int RegisterAs(char * ptrName);

/* Perform a name server query find out which PID is registered
 * as name.
 */
int WhoIs( char * ptrName);

int ValidPid( int pid);

int GetAbsMem( int tid);

int AwaitEvent();
/*
int AwaitEvent( int event);
*/

char Get(int port);

/* Get a single byte from the serial port USART number: port,
 * and return the byte read.
 */

char Put(char byte, int port);

/* Write out a single byte to serial port USART number: port */

char Write(void * msg, int len, int port);

/* Atomically write a message to the serial port, max message
 * size is MAXMSGSIZE - defined in TaskDescriptorTable.h
 */

int Read(void * msg, int port);

/* Atomically write a message to the serial port, max message
 * size is MAXMSGSIZE - defined in TaskDescriptorTable.h
 */
int UpTime(void);

/*
// read timer counter directly
//  should be used with UpTime()
*/
int GetSmallTime( void );
/* Return the number of ticks since the clock started */

int DelayUntil(int time);

/* Blcok this process until time == uptime */

inline unsigned strLen(const char *s);

#endif /* _SYS_CALL_H_ */
